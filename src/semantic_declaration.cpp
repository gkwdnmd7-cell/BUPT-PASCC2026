#include "semantic_declaration.h"

#include <utility>

namespace {
struct ArgumentInfo {
    std::shared_ptr<BasicType> type;
    bool isLvalue = false;
    SourcePosition pos;
};

class SemanticDeclarationAnalyzerImpl {
public:
    SemanticResult run(const std::vector<Token>& tokens, const ParserResult& parseResult) {
        tokens_ = &tokens;
        pos_ = 0;

        if (parseResult.root == nullptr) {
            result_.errors.push_back(SemanticError{"S199", "Syntax phase did not produce AST root.", {1, 1}});
            return result_;
        }

        TableSet* global = scopes_.push("global");
        builtins_.installInto(global);

        parseProgram();
        return result_;
    }

private:
    const std::vector<Token>* tokens_ = nullptr;
    std::size_t pos_ = 0;
    SemanticResult result_;
    ScopeStack scopes_;
    BuiltinTypePool builtins_;

    const Token& current() const {
        if (tokens_ == nullptr || tokens_->empty()) {
            static const Token kFallback{TokenType::EndOfFile, "<EOF>", {1, 1}};
            return kFallback;
        }
        if (pos_ < tokens_->size()) {
            return (*tokens_)[pos_];
        }
        return tokens_->back();
    }

    bool isAtEnd() const {
        return current().type == TokenType::EndOfFile;
    }

    void advance() {
        if (!isAtEnd()) {
            ++pos_;
        }
    }

    bool match(TokenType type) {
        if (current().type == type) {
            advance();
            return true;
        }
        return false;
    }

    TokenType peekType(std::size_t offset) const {
        if (tokens_ == nullptr || tokens_->empty()) {
            return TokenType::EndOfFile;
        }
        const std::size_t idx = pos_ + offset;
        if (idx >= tokens_->size()) {
            return TokenType::EndOfFile;
        }
        return (*tokens_)[idx].type;
    }

    void parseProgram() {
        if (!match(TokenType::KwProgram)) {
            return;
        }

        // Skip program header until ';'.
        while (!isAtEnd() && current().type != TokenType::Semicolon) {
            advance();
        }
        match(TokenType::Semicolon);

        parseBlock();
    }

    void parseBlock() {
        skipConstDeclarations();
        skipTypeDeclarations();
        parseVarDeclarationsInCurrentScope();

        while (current().type == TokenType::KwProcedure || current().type == TokenType::KwFunction) {
            parseSubprogram();
        }

        analyzeCompoundStatement();
    }

    void skipConstDeclarations() {
        if (!match(TokenType::KwConst)) {
            return;
        }
        while (!isAtEnd()) {
            if (current().type == TokenType::KwType || current().type == TokenType::KwVar || current().type == TokenType::KwProcedure ||
                current().type == TokenType::KwFunction || current().type == TokenType::KwBegin) {
                return;
            }

            if (current().type != TokenType::Identifier) {
                advance();
                continue;
            }

            const Token nameTok = current();
            advance();

            if (!match(TokenType::Equal)) {
                while (!isAtEnd() && current().type != TokenType::Semicolon &&
                       current().type != TokenType::KwVar && current().type != TokenType::KwBegin) {
                    advance();
                }
                match(TokenType::Semicolon);
                continue;
            }

            // Determine type from const value
            std::shared_ptr<BasicType> constType = builtins_.integer();
            if (current().type == TokenType::RealLiteral) {
                constType = builtins_.real();
            } else if (current().type == TokenType::CharLiteral) {
                constType = builtins_.character();
            } else if (current().type == TokenType::BooleanLiteral) {
                constType = builtins_.boolean();
            } else if (current().type == TokenType::Plus || current().type == TokenType::Minus) {
                advance();
                if (current().type == TokenType::RealLiteral) {
                    constType = builtins_.real();
                }
            } else if (current().type == TokenType::Identifier) {
                // Const defined as another identifier — inherit type if known
                auto sym = scopes_.current()->searchEntry<VariableSymbol>(current().lexeme, nullptr);
                if (sym != nullptr) {
                    auto bt = std::dynamic_pointer_cast<BasicType>(sym->type());
                    if (bt != nullptr) {
                        constType = bt;
                    }
                }
            }

            // Register the constant as a read-only variable in the current scope
            auto variable = std::make_shared<VariableSymbol>(nameTok.lexeme, constType);
            auto* mutableScope = const_cast<TableSet*>(scopes_.current());
            mutableScope->insert(nameTok.lexeme, std::move(variable));

            // Advance to the next semicolon
            while (!isAtEnd() && current().type != TokenType::Semicolon &&
                   current().type != TokenType::KwVar && current().type != TokenType::KwBegin &&
                   current().type != TokenType::KwProcedure && current().type != TokenType::KwFunction) {
                advance();
            }
            match(TokenType::Semicolon);
        }
    }

    void skipTypeDeclarations() {
        if (!match(TokenType::KwType)) {
            return;
        }
        while (!isAtEnd()) {
            if (current().type == TokenType::KwVar || current().type == TokenType::KwProcedure || current().type == TokenType::KwFunction ||
                current().type == TokenType::KwBegin) {
                return;
            }
            advance();
        }
    }

    void parseVarDeclarationsInCurrentScope() {
        if (!match(TokenType::KwVar)) {
            return;
        }

        while (!isAtEnd()) {
            if (current().type == TokenType::KwBegin || current().type == TokenType::KwProcedure || current().type == TokenType::KwFunction) {
                return;
            }

            std::vector<Token> idTokens;
            if (current().type == TokenType::Identifier) {
                idTokens.push_back(current());
                advance();
                while (match(TokenType::Comma)) {
                    if (current().type == TokenType::Identifier) {
                        idTokens.push_back(current());
                        advance();
                    } else {
                        break;
                    }
                }
            }

            if (!match(TokenType::Colon)) {
                skipToSemicolonOrBlockBoundary();
                match(TokenType::Semicolon);
                continue;
            }

            const std::string typeName = parseTypeNameAndAdvance();

            if (!typeName.empty()) {
                for (const auto& idTok : idTokens) {
                    declareVariable(idTok, typeName);
                }
            }

            match(TokenType::Semicolon);
        }
    }

    void parseSubprogram() {
        const TokenType routineKeyword = current().type;
        const bool isFunction = routineKeyword == TokenType::KwFunction;
        advance();

        Token nameToken;
        nameToken.type = TokenType::Unknown;
        nameToken.lexeme = "subprogram";
        nameToken.pos = current().pos;
        if (current().type == TokenType::Identifier) {
            nameToken = current();
            advance();
        }

        auto routine = declareRoutine(nameToken, isFunction ? RoutineSymbol::RoutineKind::Function : RoutineSymbol::RoutineKind::Procedure);

        scopes_.push(nameToken.lexeme);
        const std::vector<RoutineParameter> params = parseFormalParameters();
        if (routine != nullptr) {
            routine->setParameters(params);
        }

        if (isFunction && match(TokenType::Colon)) {
            const std::string retTypeName = parseTypeNameAndAdvance();
            if (routine != nullptr && !retTypeName.empty()) {
                bool local = false;
                const TableSet* scope = scopes_.current();
                if (scope != nullptr) {
                    auto rt = std::dynamic_pointer_cast<BasicType>(scope->searchEntry<TypeTemplate>(retTypeName, &local));
                    routine->setReturnType(rt);
                }
            }
        }

        match(TokenType::Semicolon);
        parseBlock();
        scopes_.pop();

        // Grammar has trailing ';' after subprogram body in declaration list.
        match(TokenType::Semicolon);
    }

    std::vector<RoutineParameter> parseFormalParameters() {
        std::vector<RoutineParameter> parameters;
        if (!match(TokenType::LParen)) {
            return parameters;
        }

        while (!isAtEnd() && current().type != TokenType::RParen) {
            const bool byRef = match(TokenType::KwVar);

            std::vector<Token> idTokens;
            if (current().type == TokenType::Identifier) {
                idTokens.push_back(current());
                advance();
                while (match(TokenType::Comma)) {
                    if (current().type == TokenType::Identifier) {
                        idTokens.push_back(current());
                        advance();
                    } else {
                        break;
                    }
                }
            }

            if (!match(TokenType::Colon)) {
                while (!isAtEnd() && current().type != TokenType::Semicolon && current().type != TokenType::RParen) {
                    advance();
                }
            } else {
                const std::string typeName = parseTypeNameAndAdvance();
                if (!typeName.empty()) {
                    for (const auto& idTok : idTokens) {
                        declareVariable(idTok, typeName);

                        RoutineParameter param;
                        param.byRef = byRef;
                        param.type = resolveBasicTypeByName(typeName, idTok.pos);
                        parameters.push_back(std::move(param));
                    }
                }
            }

            if (!match(TokenType::Semicolon)) {
                break;
            }
        }

        match(TokenType::RParen);
        return parameters;
    }

    void analyzeCompoundStatement() {
        if (!match(TokenType::KwBegin)) {
            return;
        }

        if (current().type != TokenType::KwEnd) {
            parseStatementSemantics();
            while (match(TokenType::Semicolon)) {
                if (current().type == TokenType::KwEnd) {
                    break;
                }
                parseStatementSemantics();
            }
        }

        match(TokenType::KwEnd);
    }

    void skipToSemicolonOrBlockBoundary() {
        while (!isAtEnd()) {
            if (current().type == TokenType::Semicolon || current().type == TokenType::KwBegin || current().type == TokenType::KwProcedure ||
                current().type == TokenType::KwFunction) {
                return;
            }
            advance();
        }
    }

    void declareVariable(const Token& idToken, const std::string& typeName) {
        const TableSet* currentScope = scopes_.current();
        if (currentScope == nullptr) {
            return;
        }

        bool localType = false;
        auto resolvedType = currentScope->searchEntry<TypeTemplate>(typeName, &localType);
        if (resolvedType == nullptr) {
            result_.errors.push_back(SemanticError{"S102", "Undefined type in declaration: " + typeName, idToken.pos});
            return;
        }

        auto variable = std::make_shared<VariableSymbol>(idToken.lexeme, resolvedType);
        auto mutableScope = const_cast<TableSet*>(currentScope);
        if (!mutableScope->insert(idToken.lexeme, std::move(variable))) {
            result_.errors.push_back(SemanticError{"S101", "Redefined identifier in same scope: " + idToken.lexeme, idToken.pos});
        }
    }

    std::shared_ptr<RoutineSymbol> declareRoutine(const Token& nameToken, RoutineSymbol::RoutineKind kind) {
        const TableSet* currentScope = scopes_.current();
        if (currentScope == nullptr) {
            return nullptr;
        }

        auto routine = std::make_shared<RoutineSymbol>(nameToken.lexeme, kind);
        auto mutableScope = const_cast<TableSet*>(currentScope);
        if (!mutableScope->insert(nameToken.lexeme, routine)) {
            result_.errors.push_back(SemanticError{"S101", "Redefined identifier in same scope: " + nameToken.lexeme, nameToken.pos});
            return nullptr;
        }
        return routine;
    }

    std::string parseTypeNameAndAdvance() {
        if (current().type == TokenType::KwArray) {
            advance();  // consume 'array'
            if (match(TokenType::LBracket)) {
                while (!isAtEnd() && current().type != TokenType::RBracket) {
                    advance();
                }
                match(TokenType::RBracket);
            }
            match(TokenType::KwOf);
            return parseTypeNameAndAdvance();  // element type
        }

        std::string typeName;
        if (current().type == TokenType::KwInteger) {
            typeName = "integer";
        } else if (current().type == TokenType::KwReal) {
            typeName = "real";
        } else if (current().type == TokenType::KwBoolean) {
            typeName = "boolean";
        } else if (current().type == TokenType::KwChar) {
            typeName = "char";
        } else if (current().type == TokenType::Identifier) {
            typeName = current().lexeme;
        }

        if (!typeName.empty()) {
            advance();
        }
        return typeName;
    }

    std::shared_ptr<BasicType> parseExpressionType() {
        auto lhs = parseSimpleExpressionType();
        if (isRelop(current().type)) {
            const Token op = current();
            advance();
            auto rhs = parseSimpleExpressionType();

            if (lhs != nullptr && rhs != nullptr) {
                const auto lk = lhs->basicKind();
                const auto rk = rhs->basicKind();
                const bool numericPair = isNumeric(lk) && isNumeric(rk);
                const bool sameKind = lk == rk;
                if (!numericPair && !sameKind) {
                    result_.errors.push_back(SemanticError{"S201", "Relational operands are not type-compatible.", op.pos});
                }
            }
            return builtins_.boolean();
        }
        return lhs;
    }

    std::shared_ptr<BasicType> parseSimpleExpressionType() {
        TokenType unary = TokenType::Unknown;
        if (current().type == TokenType::Plus || current().type == TokenType::Minus) {
            unary = current().type;
            advance();
        }

        auto resultType = parseTermType();
        if (unary != TokenType::Unknown && !isNumericType(resultType)) {
            result_.errors.push_back(SemanticError{"S201", "Unary +/- requires numeric operand.", current().pos});
        }

        while (current().type == TokenType::Plus || current().type == TokenType::Minus || current().type == TokenType::Or) {
            const TokenType op = current().type;
            const SourcePosition opPos = current().pos;
            advance();
            auto rhs = parseTermType();

            if (op == TokenType::Or) {
                if (!isBooleanType(resultType) || !isBooleanType(rhs)) {
                    result_.errors.push_back(SemanticError{"S201", "Operator 'or' requires boolean operands.", opPos});
                }
                resultType = builtins_.boolean();
            } else {
                if (!isNumericType(resultType) || !isNumericType(rhs)) {
                    result_.errors.push_back(SemanticError{"S201", "Arithmetic operator requires numeric operands.", opPos});
                }
                resultType = promoteNumeric(resultType, rhs);
            }
        }

        return resultType;
    }

    std::shared_ptr<BasicType> parseTermType() {
        auto resultType = parseFactorType();

        while (current().type == TokenType::Multiply || current().type == TokenType::Divide || current().type == TokenType::Div ||
               current().type == TokenType::Mod || current().type == TokenType::And) {
            const TokenType op = current().type;
            const SourcePosition opPos = current().pos;
            advance();
            auto rhs = parseFactorType();

            if (op == TokenType::And) {
                if (!isBooleanType(resultType) || !isBooleanType(rhs)) {
                    result_.errors.push_back(SemanticError{"S201", "Operator 'and' requires boolean operands.", opPos});
                }
                resultType = builtins_.boolean();
            } else if (op == TokenType::Div || op == TokenType::Mod) {
                if (!isIntegerType(resultType) || !isIntegerType(rhs)) {
                    result_.errors.push_back(SemanticError{"S201", "Operator 'div/mod' requires integer operands.", opPos});
                }
                resultType = builtins_.integer();
            } else if (op == TokenType::Divide) {
                if (!isNumericType(resultType) || !isNumericType(rhs)) {
                    result_.errors.push_back(SemanticError{"S201", "Operator '/' requires numeric operands.", opPos});
                }
                resultType = builtins_.real();
            } else {
                if (!isNumericType(resultType) || !isNumericType(rhs)) {
                    result_.errors.push_back(SemanticError{"S201", "Operator '*' requires numeric operands.", opPos});
                }
                resultType = promoteNumeric(resultType, rhs);
            }
        }

        return resultType;
    }

    std::shared_ptr<BasicType> parseFactorType() {
        if (current().type == TokenType::IntegerLiteral) {
            advance();
            return builtins_.integer();
        }
        if (current().type == TokenType::RealLiteral) {
            advance();
            return builtins_.real();
        }
        if (current().type == TokenType::CharLiteral) {
            advance();
            return builtins_.character();
        }
        if (current().type == TokenType::BooleanLiteral) {
            advance();
            return builtins_.boolean();
        }
        if (current().type == TokenType::Minus || current().type == TokenType::Plus) {
            advance();
            auto t = parseFactorType();
            if (t != nullptr && !isNumericType(t)) {
                // non-numeric unary — just pass through type
            }
            return t ? t : builtins_.integer();
        }
        if (current().type == TokenType::Not) {
            advance();
            auto t = parseFactorType();
            if (isIntegerType(t)) {
                return builtins_.integer();
            }
            return builtins_.boolean();
        }
        if (current().type == TokenType::LParen) {
            advance();
            auto t = parseExpressionType();
            match(TokenType::RParen);
            return t;
        }
        if (current().type == TokenType::Identifier) {
            const Token idTok = current();
            auto symbol = checkIdentifierUsage(idTok);
            advance();

            if (current().type == TokenType::LParen) {
                const auto args = parseActualArgumentsAndConsumeRParen();
                auto routine = std::dynamic_pointer_cast<RoutineSymbol>(symbol);
                if (routine == nullptr) {
                    result_.errors.push_back(SemanticError{"S201", "Identifier is not callable: " + idTok.lexeme, idTok.pos});
                    return builtins_.integer();
                }
                checkRoutineCallArguments(*routine, args, idTok.pos);
                if (routine->routineKind() == RoutineSymbol::RoutineKind::Procedure) {
                    result_.errors.push_back(SemanticError{"S202", "Procedure cannot be used as expression: " + idTok.lexeme, idTok.pos});
                    return nullptr;
                }
                return builtins_.integer();
            }

            consumeIdVarparts();
            return resolveVariableType(symbol);
        }

        return nullptr;
    }

    void parseStatementSemantics() {
        switch (current().type) {
            case TokenType::Identifier:
                parseIdentifierStatementSemantics();
                break;
            case TokenType::KwBegin:
                analyzeCompoundStatement();
                break;
            case TokenType::KwIf:
                parseIfStatementSemantics();
                break;
            case TokenType::KwWhile:
                parseWhileStatementSemantics();
                break;
            case TokenType::KwRepeat:
                parseRepeatStatementSemantics();
                break;
            case TokenType::KwFor:
                parseForStatementSemantics();
                break;
            case TokenType::KwRead:
            case TokenType::KwReadLn:
                parseReadStatementSemantics();
                break;
            case TokenType::KwWrite:
            case TokenType::KwWriteLn:
                parseWriteStatementSemantics();
                break;
            default:
                break;
        }
    }

    void parseIdentifierStatementSemantics() {
        const Token idTok = current();
        auto symbol = checkIdentifierUsage(idTok);
        advance();

        if (current().type == TokenType::LParen) {
            const auto args = parseActualArgumentsAndConsumeRParen();
            auto routine = std::dynamic_pointer_cast<RoutineSymbol>(symbol);
            if (routine == nullptr) {
                result_.errors.push_back(SemanticError{"S201", "Identifier is not callable: " + idTok.lexeme, idTok.pos});
            } else {
                checkRoutineCallArguments(*routine, args, idTok.pos);
            }
            return;
        }

        if (current().type != TokenType::Assign && current().type != TokenType::LBracket && current().type != TokenType::Dot) {
            auto routine = std::dynamic_pointer_cast<RoutineSymbol>(symbol);
            if (routine != nullptr) {
                std::vector<ArgumentInfo> noArgs;
                checkRoutineCallArguments(*routine, noArgs, idTok.pos);
            }
            return;
        }

        consumeIdVarparts();
        if (!match(TokenType::Assign)) {
            return;
        }

        auto lhsType = resolveVariableType(symbol);
        auto rhsType = parseExpressionType();
        if (!isAssignable(lhsType, rhsType)) {
            result_.errors.push_back(SemanticError{"S201", "Assignment type mismatch.", idTok.pos});
        }
    }

    void parseIfStatementSemantics() {
        const SourcePosition pos = current().pos;
        advance();
        auto cond = parseExpressionType();
        if (!isBooleanType(cond)) {
            result_.errors.push_back(SemanticError{"S201", "If condition must be boolean.", pos});
        }
        match(TokenType::KwThen);
        parseStatementSemantics();
        if (match(TokenType::KwElse)) {
            parseStatementSemantics();
        }
    }

    void parseWhileStatementSemantics() {
        const SourcePosition pos = current().pos;
        advance();
        auto cond = parseExpressionType();
        if (!isBooleanType(cond)) {
            result_.errors.push_back(SemanticError{"S201", "While condition must be boolean.", pos});
        }
        match(TokenType::KwDo);
        parseStatementSemantics();
    }

    void parseRepeatStatementSemantics() {
        const SourcePosition pos = current().pos;
        advance();
        parseStatementSemantics();
        while (match(TokenType::Semicolon)) {
            if (current().type == TokenType::KwUntil) {
                break;
            }
            parseStatementSemantics();
        }
        match(TokenType::KwUntil);
        auto cond = parseExpressionType();
        if (!isBooleanType(cond)) {
            result_.errors.push_back(SemanticError{"S201", "Repeat-until condition must be boolean.", pos});
        }
    }

    void parseForStatementSemantics() {
        const SourcePosition pos = current().pos;
        advance();

        std::shared_ptr<ObjectSymbol> iterSymbol;
        if (current().type == TokenType::Identifier) {
            iterSymbol = checkIdentifierUsage(current());
            auto iterType = resolveVariableType(iterSymbol);
            if (!isNumericType(iterType)) {
                result_.errors.push_back(SemanticError{"S201", "For iterator must be numeric variable.", current().pos});
            }
            advance();
        }

        match(TokenType::Assign);
        auto startType = parseExpressionType();
        if (!isNumericType(startType)) {
            result_.errors.push_back(SemanticError{"S201", "For start expression must be numeric.", pos});
        }

        if (current().type == TokenType::KwTo || current().type == TokenType::KwDownTo) {
            advance();
        }

        auto endType = parseExpressionType();
        if (!isNumericType(endType)) {
            result_.errors.push_back(SemanticError{"S201", "For end expression must be numeric.", pos});
        }

        match(TokenType::KwDo);
        parseStatementSemantics();
    }

    void parseReadStatementSemantics() {
        advance();
        if (!match(TokenType::LParen)) {
            return;
        }

        if (current().type != TokenType::RParen) {
            parseReadVariable();
            while (match(TokenType::Comma)) {
                parseReadVariable();
            }
        }
        match(TokenType::RParen);
    }

    void parseReadVariable() {
        if (current().type != TokenType::Identifier) {
            return;
        }
        auto symbol = checkIdentifierUsage(current());
        auto varType = resolveVariableType(symbol);
        if (varType == nullptr) {
            result_.errors.push_back(SemanticError{"S201", "read/readln requires variable argument.", current().pos});
        }
        advance();
        consumeIdVarparts();
    }

    void parseWriteStatementSemantics() {
        const TokenType kw = current().type;
        advance();

        if (!match(TokenType::LParen)) {
            return;
        }

        if (kw == TokenType::KwWriteLn && current().type == TokenType::RParen) {
            match(TokenType::RParen);
            return;
        }

        if (current().type != TokenType::RParen) {
            parseExpressionType();
            while (match(TokenType::Comma)) {
                parseExpressionType();
            }
        }
        match(TokenType::RParen);
    }

    void parseExpressionListOptAndConsumeRParen() {
        if (!match(TokenType::LParen)) {
            return;
        }

        if (current().type != TokenType::RParen) {
            parseExpressionType();
            while (match(TokenType::Comma)) {
                parseExpressionType();
            }
        }
        match(TokenType::RParen);
    }

    std::vector<ArgumentInfo> parseActualArgumentsAndConsumeRParen() {
        std::vector<ArgumentInfo> args;
        if (!match(TokenType::LParen)) {
            return args;
        }

        if (current().type != TokenType::RParen) {
            args.push_back(parseActualArgument());
            while (match(TokenType::Comma)) {
                args.push_back(parseActualArgument());
            }
        }
        match(TokenType::RParen);
        return args;
    }

    ArgumentInfo parseActualArgument() {
        ArgumentInfo arg;
        arg.pos = current().pos;
        arg.isLvalue = looksLikeVariableArgument();
        arg.type = parseExpressionType();
        return arg;
    }

    bool looksLikeVariableArgument() const {
        std::size_t idx = pos_;
        if (tokenTypeAt(idx) != TokenType::Identifier) {
            return false;
        }
        ++idx;

        while (true) {
            const TokenType t = tokenTypeAt(idx);
            if (t == TokenType::LBracket) {
                int depth = 1;
                ++idx;
                while (depth > 0) {
                    const TokenType tt = tokenTypeAt(idx);
                    if (tt == TokenType::EndOfFile) {
                        return false;
                    }
                    if (tt == TokenType::LBracket) {
                        ++depth;
                    } else if (tt == TokenType::RBracket) {
                        --depth;
                    }
                    ++idx;
                }
                continue;
            }

            if (t == TokenType::Dot && tokenTypeAt(idx + 1) == TokenType::Identifier) {
                idx += 2;
                continue;
            }
            break;
        }

        const TokenType end = tokenTypeAt(idx);
        return end == TokenType::Comma || end == TokenType::RParen;
    }

    TokenType tokenTypeAt(std::size_t idx) const {
        if (tokens_ == nullptr || idx >= tokens_->size()) {
            return TokenType::EndOfFile;
        }
        return (*tokens_)[idx].type;
    }

    void consumeIdVarparts() {
        while (true) {
            if (match(TokenType::LBracket)) {
                parseExpressionType();
                while (match(TokenType::Comma)) {
                    parseExpressionType();
                }
                match(TokenType::RBracket);
                continue;
            }
            if (match(TokenType::Dot)) {
                if (current().type == TokenType::Identifier) {
                    advance();
                }
                continue;
            }
            break;
        }
    }

    bool isRelop(TokenType type) const {
        return type == TokenType::Equal || type == TokenType::NotEqual || type == TokenType::Less || type == TokenType::LessEqual ||
               type == TokenType::Greater || type == TokenType::GreaterEqual;
    }

    bool isNumeric(BasicType::BasicKind kind) const {
        return kind == BasicType::BasicKind::Int || kind == BasicType::BasicKind::Real;
    }

    bool isNumericType(const std::shared_ptr<BasicType>& type) const {
        return type != nullptr && isNumeric(type->basicKind());
    }

    bool isBooleanType(const std::shared_ptr<BasicType>& type) const {
        return type != nullptr && type->basicKind() == BasicType::BasicKind::Bool;
    }

    bool isIntegerType(const std::shared_ptr<BasicType>& type) const {
        return type != nullptr && type->basicKind() == BasicType::BasicKind::Int;
    }

    std::shared_ptr<BasicType> promoteNumeric(const std::shared_ptr<BasicType>& lhs, const std::shared_ptr<BasicType>& rhs) {
        if (!isNumericType(lhs) || !isNumericType(rhs)) {
            return builtins_.integer();
        }
        if (lhs->basicKind() == BasicType::BasicKind::Real || rhs->basicKind() == BasicType::BasicKind::Real) {
            return builtins_.real();
        }
        return builtins_.integer();
    }

    bool isAssignable(const std::shared_ptr<BasicType>& lhs, const std::shared_ptr<BasicType>& rhs) {
        if (lhs == nullptr || rhs == nullptr) {
            return true;
        }
        if (lhs->basicKind() == rhs->basicKind()) {
            return true;
        }
        // Allow widening int -> real.
        if (lhs->basicKind() == BasicType::BasicKind::Real && rhs->basicKind() == BasicType::BasicKind::Int) {
            return true;
        }
        return false;
    }

    std::shared_ptr<BasicType> resolveVariableType(const std::shared_ptr<ObjectSymbol>& symbol) {
        if (symbol == nullptr) {
            return nullptr;
        }
        auto variable = std::dynamic_pointer_cast<VariableSymbol>(symbol);
        if (variable != nullptr) {
            return std::dynamic_pointer_cast<BasicType>(variable->type());
        }
        auto routine = std::dynamic_pointer_cast<RoutineSymbol>(symbol);
        if (routine != nullptr && routine->routineKind() == RoutineSymbol::RoutineKind::Function) {
            auto rt = routine->returnType();
            return rt ? rt : builtins_.integer();
        }
        return nullptr;
    }

    std::shared_ptr<BasicType> resolveBasicTypeByName(const std::string& typeName, const SourcePosition& pos) {
        const TableSet* currentScope = scopes_.current();
        if (currentScope == nullptr) {
            return nullptr;
        }

        bool local = false;
        auto resolved = currentScope->searchEntry<TypeTemplate>(typeName, &local);
        auto basic = std::dynamic_pointer_cast<BasicType>(resolved);
        if (basic == nullptr) {
            result_.errors.push_back(SemanticError{"S102", "Unsupported or undefined basic type: " + typeName, pos});
        }
        return basic;
    }

    void checkRoutineCallArguments(const RoutineSymbol& routine, const std::vector<ArgumentInfo>& args, const SourcePosition& pos) {
        const auto& params = routine.parameters();
        if (args.size() != params.size()) {
            result_.errors.push_back(SemanticError{"S202", "Routine argument count mismatch for: " + routine.name(), pos});
            return;
        }

        for (std::size_t i = 0; i < params.size(); ++i) {
            const auto& p = params[i];
            const auto& a = args[i];

            if (p.byRef && !a.isLvalue) {
                result_.errors.push_back(SemanticError{"S202", "Reference parameter requires lvalue argument.", a.pos});
                continue;
            }

            if (p.type == nullptr || a.type == nullptr) {
                continue;
            }

            if (p.byRef) {
                if (p.type->basicKind() != a.type->basicKind()) {
                    result_.errors.push_back(SemanticError{"S202", "Reference parameter type mismatch.", a.pos});
                }
            } else {
                if (!isAssignable(p.type, a.type)) {
                    result_.errors.push_back(SemanticError{"S202", "Value parameter type mismatch.", a.pos});
                }
            }
        }
    }

    std::shared_ptr<ObjectSymbol> checkIdentifierUsage(const Token& tok) {
        const TableSet* currentScope = scopes_.current();
        if (currentScope == nullptr) {
            return nullptr;
        }

        bool local = false;
        auto symbol = currentScope->searchEntry<ObjectSymbol>(tok.lexeme, &local);
        if (symbol == nullptr) {
            result_.errors.push_back(SemanticError{"S103", "Undefined identifier in use: " + tok.lexeme, tok.pos});
        }
        return symbol;
    }
};
}  // namespace

SemanticResult SemanticDeclarationAnalyzer::analyze(const std::vector<Token>& tokens, const ParserResult& parseResult) {
    return SemanticDeclarationAnalyzerImpl().run(tokens, parseResult);
}

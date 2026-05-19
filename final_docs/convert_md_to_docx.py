#!/usr/bin/env python3
"""
Convert markdown files (00-09) in final_docs to formatted .docx using officecli.

Formatting spec:
  - Font: 等线 (DengXian) throughout
  - Heading 1 (一级标题): bold, 四号 (14pt)
  - Heading 2 (二级标题): bold, 小四 (12pt)
  - Heading 3 (三级标题): bold, 五号 (10.5pt), not italic
  - Body text (Normal): 五号 (10.5pt), not bold, not italic
"""

import json
import re
import subprocess
import os
import sys

FONT = "等线"
BASE_DIR = r"d:\金介然\大三下\编译课设\CODE\final_docs"
OUTPUT_DIR = os.path.join(BASE_DIR, "word_output")

# Use DEVNULL to avoid encoding issues on Windows terminals
DEVNULL = subprocess.DEVNULL


def run_quiet(args, check=True):
    """Run a subprocess quietly, returning (returncode, stderr_str)."""
    result = subprocess.run(
        args,
        stdout=DEVNULL,
        stderr=subprocess.PIPE,
        encoding='utf-8',
        errors='replace'
    )
    if check and result.returncode != 0:
        err = result.stderr.strip()
        if err:
            print(f"    ERROR (exit {result.returncode}): {err[:300]}")
        raise subprocess.CalledProcessError(result.returncode, args)
    return result.returncode, (result.stderr or "")


def strip_inline_formatting(text):
    """Strip markdown inline formatting, keeping only the text content."""
    # Remove images
    text = re.sub(r'!\[.*?\]\(.*?\)', '', text)
    # Remove links, keep text
    text = re.sub(r'\[([^\]]*)\]\(.*?\)', r'\1', text)
    # Remove bold/italic markers (***, **, *)
    text = re.sub(r'\*\*\*(.+?)\*\*\*', r'\1', text)
    text = re.sub(r'\*\*(.+?)\*\*', r'\1', text)
    # Single * - careful not to match ** leftovers
    text = re.sub(r'(?<!\*)\*(?!\*)(.+?)(?<!\*)\*(?!\*)', r'\1', text)
    # Remove inline code markers
    text = re.sub(r'`([^`]+)`', r'\1', text)
    # Remove HTML tags
    text = re.sub(r'<[^>]+>', '', text)
    return text


def parse_markdown(filepath):
    """
    Parse markdown file into blocks.
    Returns list of dicts: {type, content, level?}
    Types: heading, paragraph, code_block, table_text
    """
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    blocks = []
    i = 0
    n = len(lines)

    while i < n:
        line = lines[i]
        stripped = line.strip()

        # Empty line → skip
        if stripped == '':
            i += 1
            continue

        # --- Fenced code block ---
        if stripped.startswith('```'):
            lang = stripped[3:].strip()
            code_lines = []
            i += 1
            while i < n:
                if lines[i].strip().startswith('```'):
                    i += 1
                    break
                code_lines.append(lines[i].rstrip())
                i += 1
            if code_lines:
                blocks.append({
                    'type': 'code_block',
                    'content': code_lines,
                    'lang': lang
                })
            continue

        # --- Table line (starts and ends with |) ---
        if stripped.startswith('|') and stripped.endswith('|'):
            table_rows = []
            while i < n:
                s = lines[i].strip()
                if not (s.startswith('|') and s.endswith('|')):
                    break
                # Skip separator rows like |---|---|
                if re.match(r'^\|[\s\-:|]+\|$', s):
                    i += 1
                    continue
                cells = [c.strip() for c in s.split('|')[1:-1]]
                table_rows.append(cells)
                i += 1
            if table_rows:
                blocks.append({
                    'type': 'table_text',
                    'rows': table_rows
                })
            continue

        # --- Horizontal rule ---
        if re.match(r'^(-{3,}|\*{3,}|_{3,})\s*$', stripped):
            i += 1
            continue

        # --- Heading (# through ######) ---
        heading_match = re.match(r'^(#{1,6})\s+(.+)$', stripped)
        if heading_match:
            level = len(heading_match.group(1))
            text = heading_match.group(2)
            text = strip_inline_formatting(text)
            blocks.append({
                'type': 'heading',
                'level': level,
                'content': text
            })
            i += 1
            continue

        # --- Image-only line ---
        if re.match(r'^!\[.*\]\(.*\)$', stripped):
            i += 1
            continue

        # --- Regular paragraph ---
        para_lines = []
        while i < n:
            s = lines[i].strip()
            if s == '':
                break
            if (s.startswith('#') or s.startswith('```') or
                s.startswith('|') or re.match(r'^(-{3,}|\*{3,}|_{3,})\s*$', s) or
                re.match(r'^!\[.*\]\(.*\)$', s)):
                break
            para_lines.append(s)
            i += 1

        if para_lines:
            content = ' '.join(para_lines)
            content = strip_inline_formatting(content)
            content = re.sub(r'\s+', ' ', content).strip()
            if content:
                blocks.append({
                    'type': 'paragraph',
                    'content': content
                })

    return blocks


def create_docx(md_path, docx_path):
    """Convert a single markdown file to formatted .docx."""
    md_name = os.path.basename(md_path)
    print(f"  Parsing {md_name}...", flush=True)
    blocks = parse_markdown(md_path)
    print(f"    {len(blocks)} blocks", flush=True)

    # Step 1: Create empty docx
    run_quiet(["officecli", "create", docx_path])

    # Step 2: Set document defaults
    run_quiet([
        "officecli", "set", docx_path, "/",
        "--prop", f"docDefaults.font={FONT}",
        "--prop", "docDefaults.fontSize=10.5pt"
    ])

    # Step 3: Configure styles — ADD heading styles (they don't exist yet in new docx)
    # NOTE: Do NOT pass italic=false or bold=false — officecli interprets false as true.
    # Omit italic/bold means inherits default (not italic, not bold) from Normal.
    #
    # Heading 1: bold, 四号(14pt), 等线
    run_quiet([
        "officecli", "add", docx_path, "/styles",
        "--type", "style",
        "--prop", "id=Heading1",
        "--prop", "basedOn=Normal",
        "--prop", f"font={FONT}",
        "--prop", "size=14pt",
        "--prop", "bold=true"
    ])

    # Heading 2: bold, 小四(12pt), 等线
    run_quiet([
        "officecli", "add", docx_path, "/styles",
        "--type", "style",
        "--prop", "id=Heading2",
        "--prop", "basedOn=Normal",
        "--prop", f"font={FONT}",
        "--prop", "size=12pt",
        "--prop", "bold=true"
    ])

    # Heading 3: bold, 五号(10.5pt), 等线
    run_quiet([
        "officecli", "add", docx_path, "/styles",
        "--type", "style",
        "--prop", "id=Heading3",
        "--prop", "basedOn=Normal",
        "--prop", f"font={FONT}",
        "--prop", "size=10.5pt",
        "--prop", "bold=true"
    ])

    # Normal: already exists; set font and size only.
    # Do NOT set bold/italic — default Normal has neither, which means not bold/not italic.
    run_quiet([
        "officecli", "set", docx_path, "/styles/Normal",
        "--prop", f"font={FONT}",
        "--prop", "size=10.5pt"
    ])

    # Step 4: Build batch commands
    print(f"    Building batch...", flush=True)
    commands = []

    for block in blocks:
        if block['type'] == 'heading':
            level = min(block['level'], 3)
            style = f"Heading{level}"
            commands.append({
                "op": "add",
                "parent": "/body",
                "type": "paragraph",
                "props": {
                    "text": block['content'],
                    "style": style
                }
            })

        elif block['type'] == 'paragraph':
            commands.append({
                "op": "add",
                "parent": "/body",
                "type": "paragraph",
                "props": {
                    "text": block['content'],
                    "style": "Normal"
                }
            })

        elif block['type'] == 'code_block':
            for line in block['content']:
                commands.append({
                    "op": "add",
                    "parent": "/body",
                    "type": "paragraph",
                    "props": {
                        "text": line,
                        "style": "Normal",
                        "font": "Courier New",
                        "size": "8pt"
                    }
                })

        elif block['type'] == 'table_text':
            for row in block['rows']:
                row_text = '\t'.join(row)
                commands.append({
                    "op": "add",
                    "parent": "/body",
                    "type": "paragraph",
                    "props": {
                        "text": row_text,
                        "style": "Normal"
                    }
                })

    if not commands:
        print(f"    WARNING: No content to add", flush=True)
        return True

    # Step 5: Write batch JSON
    batch_file = docx_path.replace('.docx', '_batch.json')
    with open(batch_file, 'w', encoding='utf-8') as f:
        json.dump(commands, f, ensure_ascii=False)

    file_size = os.path.getsize(batch_file)
    print(f"    Batch: {len(commands)} cmds, {file_size}B", flush=True)

    # Step 6: Execute batch (no capture to avoid encoding issues)
    result = subprocess.run(
        ["officecli", "batch", docx_path, "--input", batch_file, "--force"],
        stdout=DEVNULL,
        stderr=subprocess.PIPE,
        encoding='utf-8',
        errors='replace'
    )

    if result.returncode != 0:
        err = result.stderr.strip()
        print(f"    Batch FAILED (exit {result.returncode})")
        if err:
            print(f"      {err[:400]}")
        os.remove(batch_file)
        return False

    # Cleanup
    os.remove(batch_file)

    # Step 7: Quick verify — get paragraph count
    v_result = subprocess.run(
        ["officecli", "view", docx_path, "stats"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        encoding='utf-8',
        errors='replace'
    )
    if v_result.returncode == 0:
        for line in v_result.stdout.split('\n'):
            if 'paragraph' in line.lower() or 'Paragraph' in line:
                print(f"    {line.strip()}", flush=True)
                break

    print(f"    OK: {os.path.basename(docx_path)}", flush=True)
    return True


def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    md_files = sorted([
        f for f in os.listdir(BASE_DIR)
        if f.endswith('.md') and f[:2].isdigit()
        and f.startswith(('00', '01', '02', '03', '04', '05', '06', '07', '08', '09'))
    ])

    if not md_files:
        print("ERROR: No 00-09 markdown files found")
        sys.exit(1)

    print(f"Converting {len(md_files)} markdown files:")
    for f in md_files:
        print(f"  {f}")

    total = len(md_files)
    success = 0
    for idx, md_name in enumerate(md_files, 1):
        md_path = os.path.join(BASE_DIR, md_name)
        docx_name = md_name.replace('.md', '.docx')
        docx_path = os.path.join(OUTPUT_DIR, docx_name)
        print(f"\n[{idx}/{total}] {md_name}")
        try:
            ok = create_docx(md_path, docx_path)
            if ok:
                success += 1
        except subprocess.CalledProcessError as e:
            print(f"    FAILED: command '{e.cmd[0]}' exit {e.returncode}")
        except Exception as e:
            print(f"    FAILED: {e}")

    print(f"\n{'='*60}")
    print(f"Done: {success}/{total} files converted")
    print(f"Output: {OUTPUT_DIR}")


if __name__ == '__main__':
    main()

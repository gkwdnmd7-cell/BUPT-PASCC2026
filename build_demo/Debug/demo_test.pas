program demotest;
var a, b, result: integer;

function add(x: integer; y: integer): integer;
begin
  add := x + y
end;

begin
  a := 10;
  b := 20;
  result := add(a, b);
  write(result)
end.

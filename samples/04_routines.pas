program routines;
var x: integer;

procedure inc1(a: integer);
begin
  x := x + a
end;

function add1(v: integer): integer;
begin
  add1 := v + 1
end;

begin
  x := 1;
  inc1(2);
  x := add1(x)
end.

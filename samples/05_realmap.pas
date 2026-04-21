program realmap;
var x: real;

function addh(v: real): real;
begin
  addh := v + 0.5
end;

begin
  x := 1.25;
  x := addh(x)
end.

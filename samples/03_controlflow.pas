program ctrl;
var i, sum: integer;
begin
  i := 0;
  sum := 0;

  while i < 3 do
  begin
    if i = 1 then
      sum := sum + 10
    else
      sum := sum + i;
    i := i + 1
  end;

  for i := 1 to 2 do
    sum := sum + i;

  for i := 3 downto 1 do
    sum := sum + i
end.

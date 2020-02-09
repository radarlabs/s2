const s2 = require('bindings')('s2');

test("Cell#constructor accepts cellId", () => {
  const id = 6924439526941130752n;
  const cellId = new s2.CellId(id);
  const cell = new s2.Cell(cellId);
  expect(cell.constructor).toBe(s2.Cell);
});

test("Cell#getVertex returns Point for vertex position", () => {
  const id = 6924439526941130752n;
  const cellId = new s2.CellId(id);
  const cell = new s2.Cell(cellId);

  const v1 = cell.getVertex(0);
  const v2 = cell.getVertex(1);
  const v3 = cell.getVertex(2);
  const v4 = cell.getVertex(3);

  expect([v1.x(), v1.y(), v1.z()]).toEqual([-0.6193073896908822, 0.5249960533039503, 0.5838128990434704]);
  expect([v2.x(), v2.y(), v2.z()]).toEqual([-0.6194499844134254, 0.5251169329637175, 0.5835528455289937]);
  expect([v3.x(), v3.y(), v3.z()]).toEqual([-0.6195734250744275, 0.5248419898946621, 0.5836691328012421]);
  expect([v4.x(), v4.y(), v4.z()]).toEqual([-0.6194307451080033, 0.5247211253861704, 0.583929184566429]);
});

test("Cell#getCenter returns Point for center position", () => {
  const id = 6924439526941130752n;
  const cellId = new s2.CellId(id);
  const cell = new s2.Cell(cellId);

  const center = cell.getCenter();
  expect([center.x(), center.y(), center.z()]).toEqual([-0.6194403961888982, 0.5249190405535087, 0.5837410354204379]);
});

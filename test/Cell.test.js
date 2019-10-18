const s2 = require('bindings')('s2');

test("Cell#constructor accepts cellId", () => {
  const id = 6924439526941130752n;
  const cellId = new s2.CellId(id);
  const cell = new s2.Cell(cellId);
  expect(cell.constructor).toBe(s2.Cell);
});

test("Cell#getVertex returns LatLng for vertex position", () => {
  const id = 6924439526941130752n;
  const cellId = new s2.CellId(id);
  const cell = new s2.Cell(cellId);

  const v1 = cell.getVertex(0);
  const v2 = cell.getVertex(1);
  const v3 = cell.getVertex(2);
  const v4 = cell.getVertex(3);

  expect(v1.toString()).toEqual('35.719171,139.711561');
  expect(v2.toString()).toEqual('35.700821,139.711561');
  expect(v3.toString()).toEqual('35.709026,139.731992');
  expect(v4.toString()).toEqual('35.727378,139.731992');
});

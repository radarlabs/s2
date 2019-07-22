const s2 = require('bindings')('s2');

const tokyoTower = [35.6586, 139.7454];

test("CellId#constructor accepts id: number", () => {
  const cellId = new s2.CellId(6924439526941130752);
  expect(cellId.id()).toBe(6924439526941130752);
  expect(cellId.token()).toBe('60188d1');
  expect(cellId.level()).toBe(12);
});

test("CellId#constructor accepts ll: LatLng", () => {
  const cellId = new s2.CellId(new s2.LatLng(...tokyoTower));
  expect(cellId.id()).toBe(6924438073539271000);
  expect(cellId.token()).toBe('60188bbd9a7f993b');
  expect(cellId.level()).toBe(30);
});

test("CellId#intersects works", () => {
  const intersecting1 = new s2.CellId(6924437894853558272);
  const intersecting2 = new s2.CellId(6924437898343219200);
  const outside1 = new s2.CellId(6924438272810680320);

  expect(intersecting1.intersects(intersecting2)).toBe(true);
  expect(intersecting1.intersects(outside1)).toBe(false);

  // parent relationship
  expect(intersecting1.parent().intersects(intersecting1)).toBe(true);
});

test("CellId#contains works", () => {
  const parent = new s2.CellId(6924438358710026240);
  const child = new s2.CellId(6924439526941130752);

  expect(parent.contains(child)).toBe(true);
  expect(child.contains(parent)).toBe(false);

  // parent relationship
  expect(child.parent().contains(child)).toBe(true);
});

test("CellId#parent works", () => {
  const parent = new s2.CellId(6924438358710026240);
  const child = new s2.CellId(6924439526941130752);

  // check basic child-parent relationships
  const parentId = child.parent(parent.level()).id();
  expect(parentId).toBe(parent.id());
  expect(child.level()).toBe(12);
  expect(child.parent().level()).toBe(11);

  // level-0 cell parent should return the same cell
  const topLevelCell = new s2.CellId(8070450532247929000);
  expect(topLevelCell.level()).toBe(0);

  expect(topLevelCell.parent().id()).toBe(8070450532247929000);
  expect(topLevelCell.parent().level()).toBe(0);

  expect(topLevelCell.parent(-1).id()).toBe(8070450532247929000);
  expect(topLevelCell.parent(-1).level()).toBe(0);
});

test("CellId#child works", () => {
  const parent = new s2.CellId(12281386552583520256);

  expect(parent.level()).toBe(7);
  expect(parent.child(0).id()).toBe(12281333776025387000);
  expect(parent.child(0).level()).toBe(8);
  expect(parent.contains(parent.child(0))).toBe(true);

  expect(parent.child(1).id()).toBe(12281368960397476000);
  expect(parent.child(1).level()).toBe(8);
  expect(parent.contains(parent.child(1))).toBe(true);

  expect(parent.child(2).id()).toBe(12281404144769565000);
  expect(parent.child(2).level()).toBe(8);
  expect(parent.contains(parent.child(2))).toBe(true);

  expect(parent.child(3).id()).toBe(12281439329141654000);
  expect(parent.child(3).level()).toBe(8);
  expect(parent.contains(parent.child(3))).toBe(true);
});

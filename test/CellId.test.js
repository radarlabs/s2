const s2 = require('bindings')('s2');

const tokyoTower = [35.6586, 139.7454];


test("CellId#constructor accepts id: bigint", () => {
  const id = 6924439526941130752n;
  const cellId = new s2.CellId(id);
  expect(cellId.id()).toBe(6924439526941130752n);
  expect(cellId.idString()).toBe('6924439526941130752');
  expect(cellId.token()).toBe('60188d1');
  expect(cellId.level()).toBe(12);
});

test("CellId#constructor accepts ll: LatLng", () => {
  const cellId = new s2.CellId(new s2.LatLng(...tokyoTower));
  expect(cellId.id()).toBe(6924438073539270971n);
  expect(cellId.idString()).toBe('6924438073539270971');
  expect(cellId.token()).toBe('60188bbd9a7f993b');
  expect(cellId.level()).toBe(30);
});

test('CellId#constructor accepts token: string', () => {
  const cellId = new s2.CellId('89c258c');
  expect(cellId.id()).toBe(9926594110334959616n);
  expect(cellId.idString()).toBe('9926594110334959616');
  expect(cellId.token()).toBe('89c258c');
  expect(cellId.level()).toBe(11);
});

test('CellId#FromToken works', () => {
  const cellId = s2.CellId.fromToken('89c258c');
  expect(cellId.id()).toBe(9926594110334959616n);
  expect(cellId.idString()).toBe('9926594110334959616');
  expect(cellId.token()).toBe('89c258c');
  expect(cellId.level()).toBe(11);
});

test("CellId#intersects works", () => {
  const intersecting1 = s2.CellId.fromToken('89c2584');
  const intersecting2 = s2.CellId.fromToken('89c259');
  const outside1 = s2.CellId.fromToken('89c258c');

  expect(intersecting1.intersects(intersecting2)).toBe(true);
  expect(intersecting1.intersects(outside1)).toBe(false);

  // parent relationship
  expect(intersecting1.parent().intersects(intersecting1)).toBe(true);
});

test("CellId#contains works", () => {
  const parent = new s2.CellId(6924438358710026240n);
  const child = new s2.CellId(6924439526941130752n);

  expect(parent.contains(child)).toBe(true);
  expect(child.contains(parent)).toBe(false);

  // parent relationship
  expect(child.parent().contains(child)).toBe(true);
});

test("CellId#parent works", () => {
  const parent = new s2.CellId(6924438358710026240n);
  const child = new s2.CellId(6924439526941130752n);

  // check basic child-parent relationships
  const parentId = child.parent(parent.level()).id();
  expect(parentId).toBe(parent.id());
  expect(child.level()).toBe(12);
  expect(child.parent().level()).toBe(11);

  // level-0 cell parent should return the same cell
  const topLevelCell = s2.CellId.fromToken('9');
  expect(topLevelCell.level()).toBe(0);

  expect(topLevelCell.parent().token()).toBe('9');
  expect(topLevelCell.parent().level()).toBe(0);

  expect(topLevelCell.parent(-1).token()).toBe('9');
  expect(topLevelCell.parent(-1).level()).toBe(0);
});

test("CellId#child works", () => {
  const parent = new s2.CellId(12281386552583520256n);

  expect(parent.level()).toBe(7);
  expect(parent.child(0).id()).toBe(12281333776025387008n);
  expect(parent.child(0).level()).toBe(8);
  expect(parent.contains(parent.child(0))).toBe(true);

  expect(parent.child(1).id()).toBe(12281368960397475840n);
  expect(parent.child(1).level()).toBe(8);
  expect(parent.contains(parent.child(1))).toBe(true);

  expect(parent.child(2).id()).toBe(12281404144769564672n);
  expect(parent.child(2).level()).toBe(8);
  expect(parent.contains(parent.child(2))).toBe(true);

  expect(parent.child(3).id()).toBe(12281439329141653504n);
  expect(parent.child(3).level()).toBe(8);
  expect(parent.contains(parent.child(3))).toBe(true);

  // no further children after leaf
  const leaf = new s2.CellId(new s2.LatLng(...tokyoTower));
  expect(leaf.id()).toBe(6924438073539270971n);
  expect(leaf.level()).toBe(30);
  expect(leaf.isLeaf()).toBe(true);
  expect(leaf.child(0).id()).toBe(6924438073539270971n);
  expect(leaf.child(0).level()).toBe(30);
});

test("CellId#next works", () => {
  const cellId = new s2.CellId(6924439526941130752n);
  expect(cellId.next().id()).toBe(6924439664380084224n);
});

test("CellId#isLeaf works", () => {
  const cellId = new s2.CellId(6924439526941130752n);
  expect(cellId.isLeaf()).toBe(false);
  expect(cellId.parent(30).isLeaf()).toBe(true);
});

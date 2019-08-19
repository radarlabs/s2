const s2 = require('bindings')('s2');

// https://s2.sidewalklabs.com/regioncoverer/?center=40.719527%2C-73.960561&zoom=13&cells=89c25915%2C89c25917%2C89c25919%2C89c2593%2C89c2595%2C89c25967f%2C89c2596c%2C89c25eb4%2C89c25ecc%2C89c25ed4
const tokens = [
  '89c25915', '89c25917', '89c25919', '89c2593',
  '89c2595', '89c25967f', '89c2596c', '89c25eb4',
  '89c25ecc', '89c25ed4'
];

test("CellUnion#constructor works", () => {
  const cellIds = tokens.map(t => s2.CellId.fromToken(t));

  const cu1 = new s2.CellUnion(tokens);
  const cu2 = new s2.CellUnion(cellIds);

  expect(new Set(cu1.tokens())).toEqual(new Set(tokens)) ;
  expect(new Set(cu2.tokens())).toEqual(new Set(tokens)) ;
  expect(new Set(cu1.cellIds().map(id => id.token()))).toEqual(new Set(tokens)) ;
  expect(new Set(cu2.cellIds().map(id => id.token()))).toEqual(new Set(tokens)) ;
});

test("CellUnion#contains works", () => {
  const cu = new s2.CellUnion(tokens);

  const pointInside = s2.CellId.fromToken('89c259475f3');
  const pointOutside = s2.CellId.fromToken('89c25db57c1');

  const unionInside = new s2.CellUnion([
    '89c259380c', '89c259387', '89c25938c', '89c2593f54', '89c25940c',
    '89c259413', '89c259465', '89c25946c', '89c259474', '89c25947c'
  ]);

  const unionOutside = new s2.CellUnion([
    '89c2595', '89c25bf', '89c25c1', '89c25c3',
    '89c25c404', '89c25c6c', '89c25dd', '89c25e5c',
    '89c25e7', '89c25ec'
  ]);

  expect(cu.contains(pointInside)).toBe(true) ;
  expect(cu.contains(pointOutside)).toBe(false) ;
  expect(cu.contains(unionInside)).toBe(true) ;
  expect(cu.contains(unionOutside)).toBe(false) ;
});

test("CellUnion#intersects works", () => {
  const cu = new s2.CellUnion(tokens);

  const pointInside = s2.CellId.fromToken('89c25915');

  const unionInside = new s2.CellUnion([
    '89c259380c', '89c259387', '89c25938c', '89c2593f54',
    '89c25940c', '89c259413', '89c259465', '89c25946c',
    '89c259474', '89c25947c'
  ]);

  const unionIntersects = new s2.CellUnion([
    '89c25935','89c25937','89c25939','89c2593f','89c25944',
    '89c2594c','89c25eb34','89c25eb5','89c25ecc'
  ]);

  const unionOutside = new s2.CellUnion([
    '89c25d8c', '89c25df', '89c25e1', '89c25e3',
    '89c2604', '89c260c', '89c2614', '89c261b',
    '89c266c', '89c2674'
  ]);

  expect(cu.contains(pointInside)).toBe(true) ;
  expect(cu.contains(unionInside)).toBe(true) ;
  expect(cu.contains(unionIntersects)).toBe(true) ;
  expect(cu.contains(unionOutside)).toBe(false) ;
});

test("CellUnion#union works", () => {
  const area1 = new s2.CellUnion([
    '89c25be5','89c25be64','89c25be684','89c25bec','89c25bf4',
    '89c25bfc','89c25c01','89c25c07','89c25c09','89c25c0b'
  ]);

  const area2 = new s2.CellUnion([
    '89c25c2b','89c25c2d','89c25c34','89c25dd','89c25de4',
    '89c25dec','89c25e0c','89c25e74','89c25e7c','89c25e81'
  ]);

  const unioned = area1.union(area2);
  expect(new Set(unioned.tokens())).toEqual(new Set([
    '89c25be5','89c25be64','89c25be684','89c25bec',
    '89c25bf4','89c25bfc', '89c25c01','89c25c07',
    '89c25c09','89c25c0b','89c25c2b','89c25c2d',
    '89c25c34','89c25dd','89c25de4','89c25dec',
    '89c25e0c','89c25e74','89c25e7c','89c25e81'
  ]));
});

test("CellUnion#intersection works", () => {
  const area1 = new s2.CellUnion([
    '89c25be5','89c25be64','89c25be684','89c25bec','89c25bf4',
    '89c25bfc','89c25c01','89c25c07','89c25c09','89c25c0b'
  ]);

  const area2 = new s2.CellUnion([
    '89c25c2b','89c25c2d','89c25c34','89c25dd','89c25de4',
    '89c25dec','89c25e0c','89c25e74','89c25e7c','89c25e81'
  ]);

  // area 1 and area2 don't intersect
  const intersection1 = area1.intersection(area2);
  expect(new Set(intersection1.tokens())).toEqual(new Set());

  // area 1 should intersect after union
  const unioned = area1.union(area2);
  const intersection2 = unioned.intersection(area1);
  expect(new Set(intersection2.tokens())).toEqual(new Set(area1.tokens()));

  // area3 is inside area1
  const area3 = new s2.CellUnion([
    '89c25be5','89c25be64','89c25be684','89c25bec','89c25bf4',
  ]);
  const intersection3 = area1.intersection(area3);
  expect(new Set(intersection3.tokens())).toEqual(new Set(area3.tokens()));
});

test("CellUnion#difference works", () => {
  const area1 = new s2.CellUnion(['89c25be5','89c25be64','89c25be684','89c25bec','89c25bf4']);
  const area2 = new s2.CellUnion(['89c25be5','89c25be64','89c25be684','89c25bec','89c25bf5']);

  const difference = area1.difference(area2);

  // diff should get the 3 cells in the rest of the quadrant
  expect(new Set(difference.tokens()))
    .toEqual(new Set(['89c25bf1','89c25bf3','89c25bf7']));
});

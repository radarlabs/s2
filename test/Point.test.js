const s2 = require('bindings')('s2');

test("Point#constructor accepts x,y,z values", () => {
  const [x, y, z] = [-0.6193073896908822, 0.5249960533039503, 0.5838128990434704];
  const point = new s2.Point(x,y,z);

  expect(point.x()).toEqual(x);
  expect(point.y()).toEqual(y);
  expect(point.z()).toEqual(z);
});

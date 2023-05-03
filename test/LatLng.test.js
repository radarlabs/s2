// magic incantation from step 3 @ https://github.com/mapbox/node-pre-gyp#readme
const binary = require('@mapbox/node-pre-gyp');
const path = require('path');
var binding_path = binary.find(path.resolve('./package.json'));
const s2 = require(binding_path);

const tokyoTower = [35.6586, 139.7454];

test("LatLng#constructor accepts Point", () => {
  const point = new s2.Point(-0.6193073896908822, 0.5249960533039503, 0.5838128990434704);
  const ll = new s2.LatLng(point);
  expect(ll.toString()).toEqual('35.719171,139.711561');
});

test("LatLng#latitude works", () => {
  const ll = new s2.LatLng(...tokyoTower);
  expect(ll.latitude()).toBe(35.6586);
});

test("LatLng#longitude works", () => {
  const ll = new s2.LatLng(...tokyoTower);
  expect(ll.longitude()).toBe(139.7454);
});

test("LatLng#toString works", () => {
  const ll = new s2.LatLng(...tokyoTower);
  expect(ll.toString()).toBe('35.658600,139.745400');
});

test("LatLng#normalize works", () => {
  const denormalized = [215.6586, 319.7454];
  const ll = new s2.LatLng(...denormalized);
  const normalized = ll.normalized();
  expect(normalized.toString()).toBe('90.000000,-40.254600');
});

test("LatLng#approxEquals works", () => {
  const midLakePavilion = [30.248566, 120.139686];
  const midLakePavilion2 = [30.248569, 120.139686];

  const ll1 = new s2.LatLng(...midLakePavilion);
  const ll2 = new s2.LatLng(...midLakePavilion2);

  expect(ll1.approxEquals(ll2, 1e-7)).toBe(true);
});

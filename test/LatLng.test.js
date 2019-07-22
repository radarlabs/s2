const s2 = require('bindings')('s2');

const tokyoTower = [35.6586, 139.7454];

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

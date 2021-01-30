// magic incantation from step 3 @ https://github.com/mapbox/node-pre-gyp#readme
const binary = require('node-pre-gyp');
const path = require('path');
var binding_path = binary.find(path.resolve('./package.json'));
const s2 = require(binding_path);

const westLakeUniversity = [30.135703, 120.075485];
const apsaraPark = [30.135770, 120.074877];

test("Earth#constructor", () => {
  const earth = new s2.Earth();
  expect(earth.constructor).toBe(s2.Earth);
});


test("Earth#toMeters Earth#getDistanceMeters Earth#toKm Earth#getDistanceKm Earth#getRadians Earth#getDegrees", () => {
  const latlng1 = new s2.LatLng(westLakeUniversity[0], westLakeUniversity[1]);
  const latlng2 = new s2.LatLng(apsaraPark[0], apsaraPark[1]);

  const distanceToMeters = s2.Earth.toMeters(latlng1, latlng2);
  const distanceMeters = s2.Earth.getDistanceMeters(latlng1, latlng2);

  const distanceToKm = s2.Earth.toKm(latlng1, latlng2);
  const distanceKm = s2.Earth.getDistanceKm(latlng1, latlng2);

  const radians = s2.Earth.getRadians(latlng1, latlng2);
  const degrees = s2.Earth.getDegrees(latlng1, latlng2);

  expect(distanceToMeters).toBe(58.941537292477996);
  expect(distanceMeters).toBe(58.941537292477996);
  expect(distanceToKm).toBe(0.058941537292477996);
  expect(distanceKm).toBe(0.058941537292477996);

  expect(radians).toBe(0.00000925152170416904);
  expect(degrees).toBe(0.0005300731477225649);
});

test("Earth#getInitalBearingDegrees", () => {
  const centerLatLng = [30.135703, 120.075485];
  const eastLatLng = [30.135717, 120.076004];
  const southLatLng = [30.135510, 120.075515];
  const westLatLng = [30.135749, 120.075085];
  const northLatLng = [30.136150, 120.075611];

  const center = new s2.LatLng(centerLatLng[0], centerLatLng[1]);
  const east = new s2.LatLng(eastLatLng[0], eastLatLng[1]);
  const south = new s2.LatLng(southLatLng[0], southLatLng[1]);
  const west = new s2.LatLng(westLatLng[0], westLatLng[1]);
  const north = new s2.LatLng(northLatLng[0], northLatLng[1]);

  const degreeEast = s2.Earth.getInitalBearingDegrees(center, east);
  expect(degreeEast).toBe(88.21335159225947);

  const degreeSouth = s2.Earth.getInitalBearingDegrees(center, south);
  expect(degreeSouth).toBe(172.34356352357213);

  const degreeWest = s2.Earth.getInitalBearingDegrees(center, west);
  expect(degreeWest).toBe(-82.42555355986279);

  const degreeNorth = s2.Earth.getInitalBearingDegrees(center, north);
  expect(degreeNorth).toBe(13.70028061338925);
});

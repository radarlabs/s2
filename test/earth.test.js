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


test("Earth#toMeters", () => {
  const latlng1 = new s2.LatLng(westLakeUniversity[0], westLakeUniversity[1]);
  const latlng2 = new s2.LatLng(apsaraPark[0], apsaraPark[1]);
  
  const distance_tometers = s2.Earth.toMeters(latlng1, latlng2);
  const distance = s2.Earth.getDistanceMeters(latlng1, latlng2);
  const radian = s2.Earth.getRadian(latlng1, latlng2);
  const degree = s2.Earth.getDegree(latlng1, latlng2);

  //console.log('----- distance_tometers: %s, radian: %s, degree: %s, distance: %s', distance_tometers, radian, degree, distance);

  expect(distance_tometers).toBe(58.941537292477996);
  expect(distance).toBe(58.941537292477996);
  expect(radian).toBe(0.00000925152170416904);
  expect(degree).toBe(0.0005300731477225649);
});

test("Earth#getInitalBearingDegree", () => {
  //[30.135703, 120.075485],[30.135717, 120.076004],[30.135510, 120.075515],[30.135749, 120.075085],[30.136150, 120.075611]
  const center = [30.135703, 120.075485];
  const east = [30.135717, 120.076004];
  const south = [30.135510, 120.075515];
  const west = [30.135749, 120.075085];
  const north = [30.136150, 120.075611];

  const latLng_center = new s2.LatLng(center[0], center[1]);
  const latLng_east = new s2.LatLng(east[0], east[1]);
  const latLng_south = new s2.LatLng(south[0], south[1]);
  const latLng_west = new s2.LatLng(west[0], west[1]);
  const latLng_north = new s2.LatLng(north[0], north[1]);

  
  const degree_east = s2.Earth.getInitalBearingDegree(latLng_center, latLng_east);
  expect(degree_east).toBe(88.21335159225947);

  const degree_south = s2.Earth.getInitalBearingDegree(latLng_center, latLng_south);
  expect(degree_south).toBe(172.34356352357213);

  const degree_west = s2.Earth.getInitalBearingDegree(latLng_center, latLng_west);
  expect(degree_west).toBe(-82.42555355986279);

  const degree_north = s2.Earth.getInitalBearingDegree(latLng_center, latLng_north);
  expect(degree_north).toBe(13.70028061338925);

});

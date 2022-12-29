// magic incantation from step 3 @ https://github.com/mapbox/node-pre-gyp#readme
const binary = require('@mapbox/node-pre-gyp');
const path = require('path');
var binding_path = binary.find(path.resolve('./package.json'));
const s2 = require(binding_path);

const PRECISION = 6;

const baiCauseway = [[30.261254, 120.147342],[30.260202, 120.146395],[30.258967, 120.145267],[30.257545, 120.144089], [30.256662, 120.143349],[30.256027, 120.142843],[30.255445, 120.142336]]; // 白堤。 WGS84
const polyline = new s2.Polyline(baiCauseway.map((latlng) => {
  const [lat, lng] = latlng;
  return new s2.LatLng(lat, lng);
}));

test("Polyline#contains", () => {
  const [lat, lng] = baiCauseway[1];
  const cellId = new s2.CellId(new s2.LatLng(lat, lng));
  const cell = new s2.Cell(cellId);
  expect(polyline.contains(cell)).toBe(false);
});

test("Polyline#nearlyCovers true", () => {
  const newBaiCauseway = [[30.261254, 120.147342],[30.260202, 120.146395],[30.258967, 120.145267],[30.257545, 120.144089], [30.256662, 120.143349],[30.256027, 120.142843],[30.255445, 120.142336],[30.254703, 120.141366]];
  const newLLs = newBaiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const newPolyline = new s2.Polyline(newLLs);

  expect(polyline.nearlyCovers(newPolyline, 1e-5)).toBe(false);
  expect(newPolyline.nearlyCovers(polyline, 1e-5)).toBe(true);
});

test("Polyline#nearlyCovers false", () => {
  const tempPoints = [[30.261076, 120.153102],[30.259816, 120.154280], [30.258923, 120.152020],[30.258824, 120.151923 ],[30.258739, 120.151840],[30.258642, 120.151754]];
  const newBaiCauseway = tempPoints.concat([]);
  const newLLs = newBaiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const newPolyline = new s2.Polyline(newLLs);

  expect(polyline.nearlyCovers(newPolyline, 1e-5)).toBe(false);
});

test("Polyline#getLength", () => {
  expect(polyline.getLength().toFixed(PRECISION)).toBe("805.441648");
});

test("Polyline#getCentroid", () => {
  const centroid = polyline.getCentroid();
  expect(centroid.latitude().toFixed(PRECISION)).toBe("30.258369");
  expect(centroid.longitude().toFixed(PRECISION)).toBe("120.144809");
});

test("Polyline#interpolate", () => {
  const interpolated = polyline.interpolate(0.75);
  expect(interpolated.latitude().toFixed(PRECISION)).toBe("30.256914");
  expect(interpolated.longitude().toFixed(PRECISION)).toBe("120.143561");
});

test("Polyline#project", () => {
  const westLakeMuseum = [30.259710, 120.140802];// 西湖博览会博物馆
  const westLakeMuseumLatLng = new s2.LatLng(westLakeMuseum[0], westLakeMuseum[1]);

  const westLakeMuseumNearPoint = polyline.project(westLakeMuseumLatLng);
  expect(westLakeMuseumNearPoint.point.latitude().toFixed(PRECISION)).toBe("30.257633");
  expect(westLakeMuseumNearPoint.point.longitude().toFixed(PRECISION)).toBe("120.144162");
  expect(westLakeMuseumNearPoint.index).toBe(3);

  const midLakePavilion = [30.248636, 120.139723]; // 西湖湖心亭
  const midLakePavilionLatLng = new s2.LatLng(midLakePavilion[0], midLakePavilion[1]);

  const midLakePavilionPoint = polyline.project(midLakePavilionLatLng);
  expect(midLakePavilionPoint.point.latitude().toFixed(PRECISION)).toBe("30.255445");
  expect(midLakePavilionPoint.point.longitude().toFixed(PRECISION)).toBe("120.142336");
  expect(midLakePavilionPoint.index).toBe(7);
})

// magic incantation from step 3 @ https://github.com/mapbox/node-pre-gyp#readme
const binary = require('node-pre-gyp');
const path = require('path');
var binding_path = binary.find(path.resolve('./package.json'));
const s2 = require(binding_path);

const westLake = [30.248566, 120.139686]; // 湖心亭
//const baiCauseway = [[30.258923, 120.152020],[30.258824, 120.151923 ],[30.258739, 120.151840],[30.258642, 120.151754], [30.258513, 120.151633], [30.258308, 120.151448],[30.258224, 120.151374] , [30.257902, 120.151074], [30.257284, 120.150508], [30.256237, 120.149607], [30.255449, 120.148958],[30.255449, 120.148958],[30.255005, 120.148592],[30.254763, 120.148385],[30.254317, 120.148000]]; // 白堤。GCJ02

const baiCauseway = [[30.261254, 120.147342],[30.260202, 120.146395],[30.258967, 120.145267],[30.257545, 120.144089], [30.256662, 120.143349],[30.256027, 120.142843],[30.255445, 120.142336]]; // 白堤。 WGS84

const baiCauseway_point = [30.257775, 120.144291];

test("Polyline#constructor accepts LatLng array", () => {
  
  const lls = baiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const polyline = new s2.Polyline(lls);
  expect(polyline.constructor).toBe(s2.Polyline);
});

test("Polyline#contain", () => {
  
  const lls = baiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const polyline = new s2.Polyline(lls);

  const [lat, lng] = baiCauseway[1];//baiCauseway_point;
  const cellId = new s2.CellId(new s2.LatLng(lat, lng));
  const cell = new s2.Cell(cellId);

  expect(polyline.contains(cell)).toBe(false);
});

test("Polyline#NearlyCovers true", () => {
  
  const lls = baiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const polyline = new s2.Polyline(lls);


  const newBaiCauseway = [[30.261254, 120.147342],[30.260202, 120.146395],[30.258967, 120.145267],[30.257545, 120.144089], [30.256662, 120.143349],[30.256027, 120.142843],[30.255445, 120.142336],[30.254703, 120.141366]];
  const newLls = newBaiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const newPolyline = new s2.Polyline(newLls);

  expect(polyline.nearlyCovers(newPolyline, 1e-5)).toBe(false);
  expect(newPolyline.nearlyCovers(polyline, 1e-5)).toBe(true);
});

test("Polyline#NearlyCovers false", () => {
  
  const lls = baiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const polyline = new s2.Polyline(lls);

  const tempPoints = [[30.261076, 120.153102],[30.259816, 120.154280], [30.258923, 120.152020],[30.258824, 120.151923 ],[30.258739, 120.151840],[30.258642, 120.151754]];
  const newBaiCauseway = tempPoints.concat([]);
  const newLls = newBaiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const newPolyline = new s2.Polyline(newLls); 

  expect(polyline.nearlyCovers(newPolyline, 1e-5)).toBe(false);
});

test("Polyline#getLength", () => {
  const lls = baiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const polyline = new s2.Polyline(lls);

  expect(polyline.getLength()).toBe(805.4416481053961);
});

test("Polyline#getCentroid", () => {
  const lls = baiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const polyline = new s2.Polyline(lls);
  const latlngStr = polyline.getCentroid();
  //console.log("getCentroid: ", latlngStr);
  
  expect(polyline.getCentroid()).toBe("30.258369,120.144809");
});

test("Polyline#interpolate", () => {
  const lls = baiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });

  const polyline = new s2.Polyline(lls);
  const latlngStr = polyline.interpolate(3/4);
  //console.log('interpolate: latlngStr: ', latlngStr);
  
  expect(polyline.interpolate(3/4)).toBe("30.256914,120.143561");
});

test("poly#project", () => {
  // baiCauseway polyline
  const lls = baiCauseway.map((latlng) => {
    const [lat, lng] = latlng;
    return new s2.LatLng(lat, lng);
  });
  const polyline = new s2.Polyline(lls);

  const westLakeMuseum = [30.259710, 120.140802];//西湖博览会博物馆
  const westLakeMuseumLatLng = new s2.LatLng(westLakeMuseum[0], westLakeMuseum[1]);
  expect(polyline.project(westLakeMuseumLatLng)).toBe("30.257633,120.144162");

  const midLakePavilion = [30.248636, 120.139723];// 西湖湖心亭
  const midLakePavilionLatLng = new s2.LatLng(midLakePavilion[0], midLakePavilion[1]);

  expect(polyline.project(midLakePavilionLatLng)).toBe("30.255445,120.142336");
  // console.log('midLakePavilionLatLng: ', polyline.project(midLakePavilionLatLng));
  // console.log('westLakeMuseumLatLng: ', polyline.project(westLakeMuseumLatLng));
})



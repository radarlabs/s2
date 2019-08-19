const s2 = require('bindings')('s2');

// long, lat pairs of 11201 postal code
const postalCode11201 = [[-73.995069,40.703102],[-73.995138,40.70298],[-73.996698,40.700877],[-73.9975,40.699653],[-73.998032,40.698762],[-73.998376,40.698048],[-73.998847,40.697141],[-73.998763,40.69711],[-73.998857,40.69692],[-73.99894,40.696941],[-73.998991,40.696812],[-73.999182,40.696885],[-73.999295,40.696795],[-73.999474,40.696704],[-73.999535,40.696578],[-73.999478,40.696466],[-73.999428,40.69641],[-73.99942,40.696361],[-73.999461,40.69628],[-73.999604,40.696069],[-73.999882,40.695605],[-74.000277,40.694959],[-74.000701,40.694388],[-74.000959,40.694069],[-74.001194,40.693484],[-74.001277,40.693298],[-74.001317,40.693208],[-74.001542,40.692786],[-74.001744,40.692407],[-74.001818,40.692222],[-74.001969,40.691843],[-74.001332,40.691225],[-74.000699,40.690612],[-74.00096,40.690132],[-74.001853,40.690388],[-74.001963,40.690136],[-74.002178,40.690206],[-74.002427,40.690273],[-74.002616,40.690324],[-74.002763,40.690364],[-74.002993,40.690427],[-74.003024,40.690349],[-74.003409,40.689613],[-74.00382,40.68893],[-74.003919,40.68886],[-74.004593,40.688215],[-74.005038,40.687561],[-74.00563,40.686784],[-74.005874,40.686207],[-74.00635,40.686285],[-74.006406,40.686289],[-74.006444,40.686296],[-74.006506,40.686314],[-74.006594,40.686347],[-74.006755,40.686413],[-74.006875,40.686464],[-74.006989,40.686509],[-74.00702,40.686526],[-74.007064,40.686504],[-74.00715,40.686475],[-74.007243,40.686449],[-74.007411,40.686388],[-74.007497,40.686344],[-74.007536,40.686315],[-74.007623,40.686285],[-74.00767,40.686269],[-74.00773,40.686254],[-74.007778,40.686245],[-74.007843,40.686238],[-74.007899,40.686251],[-74.007929,40.686276],[-74.00795,40.686293],[-74.008164,40.686175],[-74.008425,40.686016],[-74.008386,40.685969],[-74.008135,40.685615],[-74.005497,40.685847],[-74.004853,40.685668],[-74.004504,40.686301],[-74.002364,40.685737],[-74.002039,40.686404],[-74.001718,40.687069],[-74.001389,40.687751],[-74.001278,40.687715],[-74.000539,40.687481],[-74.000054,40.687318],[-73.999962,40.687286],[-73.999863,40.687251],[-73.999696,40.687198],[-73.998932,40.686948],[-73.998276,40.686729],[-73.998189,40.6867],[-73.997278,40.686398],[-73.996488,40.686138],[-73.996358,40.686096],[-73.99618,40.686037],[-73.995386,40.685775],[-73.994538,40.685495],[-73.994408,40.685453],[-73.994578,40.6851],[-73.992192,40.684178],[-73.991952,40.684084],[-73.989992,40.683319],[-73.989556,40.683956],[-73.989129,40.68459],[-73.988712,40.685224],[-73.988289,40.685869],[-73.987859,40.686504],[-73.987429,40.687138],[-73.987,40.687773],[-73.986568,40.688408],[-73.9844,40.687564],[-73.983988,40.688175],[-73.983572,40.688786],[-73.982715,40.688459],[-73.982416,40.688896],[-73.982098,40.689358],[-73.981068,40.688945],[-73.98087,40.688866],[-73.980258,40.688638],[-73.980429,40.689901],[-73.97951,40.689863],[-73.978624,40.689831],[-73.977939,40.689806],[-73.977098,40.689773],[-73.976102,40.689737],[-73.975106,40.689698],[-73.974109,40.689663],[-73.973264,40.689627],[-73.973126,40.689622],[-73.97354,40.691707],[-73.973855,40.693287],[-73.97422,40.693301],[-73.974985,40.693329],[-73.975848,40.693361],[-73.977827,40.693438],[-73.979168,40.693489],[-73.979195,40.693695],[-73.979218,40.693765],[-73.979259,40.69382],[-73.97933,40.693891],[-73.979481,40.694013],[-73.979805,40.694241],[-73.979883,40.694308],[-73.979944,40.694385],[-73.980139,40.694683],[-73.980243,40.695457],[-73.980263,40.695634],[-73.980356,40.696178],[-73.979537,40.696172],[-73.979529,40.696298],[-73.979524,40.696381],[-73.979529,40.696529],[-73.980388,40.696507],[-73.980447,40.69667],[-73.980515,40.697437],[-73.980544,40.698246],[-73.980462,40.699703],[-73.980455,40.700852],[-73.980729,40.701268],[-73.980649,40.701561],[-73.980455,40.702248],[-73.980224,40.703063],[-73.979988,40.703541],[-73.979603,40.704177],[-73.979466,40.704462],[-73.979328,40.704746],[-73.979168,40.705279],[-73.979171,40.705301],[-73.979372,40.705443],[-73.979233,40.705769],[-73.979479,40.705825],[-73.980332,40.705846],[-73.980907,40.705907],[-73.980987,40.705374],[-73.981856,40.70549],[-73.982033,40.705515],[-73.98221,40.70554],[-73.982329,40.705552],[-73.982449,40.705547],[-73.982455,40.705509],[-73.982572,40.705518],[-73.982748,40.705532],[-73.982741,40.705576],[-73.982825,40.705558],[-73.98308,40.705519],[-73.98308,40.70548],[-73.983227,40.705482],[-73.983351,40.705451],[-73.983398,40.705467],[-73.983426,40.705577],[-73.983442,40.705645],[-73.984189,40.705582],[-73.985852,40.705415],[-73.986434,40.705349],[-73.986429,40.705083],[-73.98653,40.704979],[-73.986623,40.704979],[-73.986641,40.705018],[-73.98669,40.705075],[-73.986762,40.705118],[-73.986866,40.705161],[-73.987271,40.705181],[-73.987449,40.705166],[-73.987577,40.705158],[-73.987957,40.705134],[-73.989224,40.705137],[-73.989424,40.705137],[-73.989495,40.705145],[-73.989608,40.705158],[-73.989861,40.705175],[-73.990549,40.705144],[-73.990696,40.705131],[-73.990882,40.705124],[-73.99148,40.705625],[-73.992722,40.705544],[-73.993853,40.704829],[-73.993713,40.70452],[-73.993693,40.704476],[-73.993775,40.704449],[-73.993807,40.70454],[-73.993861,40.704556],[-73.994341,40.704429],[-73.994498,40.704318],[-73.994565,40.704265],[-73.994618,40.704226],[-73.994797,40.704096],[-73.994714,40.703997],[-73.994816,40.703906],[-73.994881,40.703813],[-73.994882,40.703788],[-73.994992,40.703654],[-73.994858,40.703566],[-73.994663,40.703431],[-73.994806,40.703296],[-73.995069,40.703102]];
const unnormalized = [
  [-119.74242645808715,39.57982985652839],
  [-119.74457144737244,39.579904675483704],
  [-119.7435238,39.5835421],
  [-119.7378725,39.5808519],
  [-119.7393411397934,39.57999050617218],
  [-119.74242645808715,39.57982985652839]
];

test("RegionCoverer#getCoveringTokens works", () => {
  const lls = postalCode11201.map((latlng) => {
    const [lng, lat] = latlng;
    return new s2.LatLng(lat, lng);
  });

  // Invalid loop with duplicate points (first and last are the same)
  expect(() => new s2.RegionCoverer.getCoveringTokens(null, {})).toThrow('s2.LatLng[]');
  expect(() => new s2.RegionCoverer.getCoveringTokens([], {})).toThrow('was empty');
  expect(() => new s2.RegionCoverer.getCoveringTokens(lls, {})).toThrow('duplicate vertex');

  // level-14 s2 cells
  let covering = s2.RegionCoverer.getCoveringTokens(lls.slice(1), { min: 14, max: 14 });
  let coveringTokens = new Set(covering);
  let expectedTokens = new Set([
    '89c25a31',
    '89c25a33',
    '89c25a35',
    '89c25a37',
    '89c25a39',
    '89c25a3b',
    '89c25a41',
    '89c25a43',
    '89c25a45',
    '89c25a47',
    '89c25a49',
    '89c25a4b',
    '89c25a4d',
    '89c25a4f',
    '89c25a51',
    '89c25a53',
    '89c25a5b',
    '89c25a5d',
    '89c25a67',
    '89c25bb1',
    '89c25bb3',
    '89c25bb5',
    '89c25bb7',
    '89c25bcb',
    '89c25bcd',
    '89c25bd3'
  ]);

  expect(coveringTokens).toEqual(expectedTokens);

  // levels 12-20
  covering = s2.RegionCoverer.getCoveringTokens(lls.slice(1), { min: 12, max: 20 });
  coveringTokens = new Set(covering);

  expect(coveringTokens).toEqual(new Set([
    '89c25a34',
    '89c25a3c',
    '89c25a5',
    '89c25a674',
    '89c25bb4',
    '89c25bcb',
    '89c25bcd',
    '89c25bd3'
  ]));
});

test("RegionCoverer can cover unnormalized loops", () => {
  const lls = unnormalized.map((latlng) => {
    const [lng, lat] = latlng;
    return new s2.LatLng(lat, lng);
  });

  // generate s2 cells
  let covering = s2.RegionCoverer.getCoveringTokens(lls.slice(1), { min: 14, max: 20 });
  let coveringTokens = new Set(covering);
  let expectedTokens = new Set([
    '809938fe4',
    '809938ffc',
    '809939004',
    '809939014',
    '80993901c',
    '809939021f',
    '809939023',
    '80993903c',
  ]);

  expect(coveringTokens).toEqual(expectedTokens);

});

test("RegionCoverer#getCovering works", () => {
  const lls = postalCode11201.map((latlng) => {
    const [lng, lat] = latlng;
    return new s2.LatLng(lat, lng);
  });

  // Invalid loop with duplicate points (first and last are the same)
  expect(() => new s2.RegionCoverer.getCovering(null, {})).toThrow('s2.LatLng[]');
  expect(() => new s2.RegionCoverer.getCovering([], {})).toThrow('was empty');
  expect(() => new s2.RegionCoverer.getCovering(lls, {})).toThrow('duplicate vertex');

  // level-14 s2 cells
  let covering = s2.RegionCoverer.getCovering(lls.slice(1), { min: 14, max: 14 });
  let coveringCellIds = covering.cellIds();
  let coveringTokens = new Set(covering.tokens());
  let expectedTokens = new Set([
    '89c25a31',
    '89c25a33',
    '89c25a35',
    '89c25a37',
    '89c25a39',
    '89c25a3b',
    '89c25a41',
    '89c25a43',
    '89c25a45',
    '89c25a47',
    '89c25a49',
    '89c25a4b',
    '89c25a4d',
    '89c25a4f',
    '89c25a51',
    '89c25a53',
    '89c25a5b',
    '89c25a5d',
    '89c25a67',
    '89c25bb1',
    '89c25bb3',
    '89c25bb5',
    '89c25bb7',
    '89c25bcb',
    '89c25bcd',
    '89c25bd3'
  ]);

  expect(coveringTokens).toEqual(expectedTokens);
  expect(new Set(coveringCellIds.map(id => id.token()))).toEqual(expectedTokens);

  // levels 12-20
  covering = s2.RegionCoverer.getCovering(lls.slice(1), { min: 12, max: 20 });
  coveringCellIds = covering.cellIds();
  coveringTokens = new Set(covering.tokens());

  expectedTokens = new Set([
    '89c25a34',
    '89c25a3c',
    '89c25a5',
    '89c25a674',
    '89c25bb4',
    '89c25bcb',
    '89c25bcd',
    '89c25bd3'
  ]);

  expect(coveringTokens).toEqual(expectedTokens);
  expect(new Set(coveringCellIds.map(id => id.token()))).toEqual(expectedTokens);
});

const s2 = require('..');

// Snapshots were captured from the original S2Builder-based implementation.
const polygons = [
  ['local rectangle', [[40.70, -74], [40.70, -73.99], [40.71, -73.99], [40.71, -74]]],
  ['concave ring', [[0, 0], [0, 1], [0.5, 0.5], [1, 1], [1, 0]]],
  ['antimeridian', [[10, 179.9], [10, -179.9], [10.1, -179.9], [10.1, 179.9]]],
  ['north pole', [[85, -135], [85, -45], [85, 45], [85, 135]]],
  ['south pole', [[-85, -135], [-85, -45], [-85, 45], [-85, 135]]],
  ['cube face boundary', [[-0.01, 44.99], [-0.01, 45.01], [0.01, 45.01], [0.01, 44.99]]],
  ['equator', [[0, 0], [0, 90], [0, 180], [0, -90]]],
  ['tiny triangle', [[40, -74], [40, -74 + 1e-13], [40 + 1e-13, -74]]],
  ['normalized coordinates', [[10, 539.9], [10, 180.1], [10.1, 180.1], [10.1, 539.9]]],
  ['single vertex north', [[40, -74]]],
  ['single vertex south', [[-40, -74]]],
];

function expectFormats(points, options, tokens) {
  expect(s2.RegionCoverer.getCoveringTokens(points, options)).toEqual(tokens);
  const ids = tokens.map(token => new s2.CellId(token).id());
  expect(Array.from(s2.RegionCoverer.getCoveringIds(points, options))).toEqual(ids);
  const union = s2.RegionCoverer.getCovering(points, options);
  expect(union.tokens()).toEqual(tokens);
  expect(Array.from(union.ids())).toEqual(ids);
}

describe.each(polygons)('Polygon covering: %s', (name, coordinates) => {
  test.each([false, true])('preserves original covering (reversed=%s)', reversed => {
    const points = (reversed ? [...coordinates].reverse() : coordinates)
      .map(([lat, lng]) => new s2.LatLng(lat, lng));
    const options = { max: 16, max_cells: 12 };
    const tokens = s2.RegionCoverer.getCoveringTokens(points, options);
    expect(tokens).toMatchSnapshot();
    expectFormats(points, options, tokens);
  });
});

test('Fixed-level polygon coverings preserve every cell at the requested level', () => {
  const points = polygons[0][1].map(([lat, lng]) => new s2.LatLng(lat, lng));
  const options = { min: 14, max: 14, max_cells: 1 };
  const tokens = s2.RegionCoverer.getCoveringTokens(points, options);
  expect(tokens).toMatchSnapshot();
  expect(tokens.length).toBeGreaterThan(1);
  expect(tokens.every(token => new s2.CellId(token).level() === 14)).toBe(true);
  expectFormats(points, options, tokens);
});

test.each([
  ['two vertices', [[0, 0], [0, 1]], /Loop is invalid: 100/],
  ['closing duplicate', [[0, 0], [0, 1], [1, 1], [0, 0]], /duplicate vertex/],
  ['nonadjacent duplicate', [[0, 0], [0, 1], [1, 1], [0, 1], [1, 0]], /duplicate vertex/],
  ['self crossing', [[0, 0], [1, 1], [0, 1], [1, 0]], /crosses/],
])('All polygon APIs reject %s', (name, coordinates, error) => {
  const points = coordinates.map(([lat, lng]) => new s2.LatLng(lat, lng));
  for (const method of ['getCoveringTokens', 'getCoveringIds', 'getCovering']) {
    expect(() => s2.RegionCoverer[method](points, {})).toThrow(error);
  }
});

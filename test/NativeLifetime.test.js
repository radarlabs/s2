const { execFileSync } = require('node:child_process');
const path = require('node:path');

// Isolate native crashes and RSS measurements, and expose GC without requiring
// callers to change how they run Jest. Pass code as an argument, not via a shell.
function runWithGC(check) {
  return execFileSync(process.execPath, [
    '--expose-gc',
    '-e',
    `(${check.toString()})(${JSON.stringify(path.resolve(__dirname, '..'))}).catch(error => {
      console.error(error);
      process.exitCode = 1;
    });`,
  ], { encoding: 'utf8', timeout: 20000 });
}

test('Native CellUnion results survive collection of their inputs', () => {
  runWithGC(async root => {
    const assert = require('node:assert/strict');
    const s2 = require(root);

    function makeResults() {
      const left = new s2.CellUnion(['1', '3']);
      const right = new s2.CellUnion(['3', '5']);
      const points = [[40, -74], [40, -73.99], [40.01, -73.99], [40.01, -74]]
        .map(([lat, lng]) => new s2.LatLng(lat, lng));
      const center = new s2.LatLng(40, -74);
      const options = { min: 12, max: 14 };
      return {
        inputs: [left, right, ...points, center].map(input => new WeakRef(input)),
        results: [
          [left.union(right), ['1', '3', '5']],
          [left.intersection(right), ['3']],
          [left.difference(right), ['1']],
          [left.difference(left), []],
          [s2.RegionCoverer.getCovering(points, options),
            s2.RegionCoverer.getCoveringTokens(points, options)],
          [s2.RegionCoverer.getRadiusCovering(center, 1000, options),
            s2.RegionCoverer.getRadiusCoveringTokens(center, 1000, options)],
        ],
      };
    }

    const { inputs, results } = makeResults();
    for (let round = 0; round < 3; round++) {
      // Leave the job that created the WeakRefs, then allow native finalizers
      // to run before accessing the retained results.
      await new Promise(resolve => setImmediate(resolve));
      global.gc();
      await new Promise(resolve => setImmediate(resolve));
    }
    assert(inputs.every(input => input.deref() === undefined), 'inputs must be collected');

    for (const [result, tokens] of results) {
      assert.deepEqual(result.tokens(), tokens);
      assert.deepEqual(result.cellIds().map(cell => cell.token()), tokens);
      assert.deepEqual(Array.from(result.ids()), tokens.map(token => new s2.CellId(token).id()));
      assert.deepEqual(result.union(result).tokens(), tokens);
      assert.deepEqual(result.intersection(result).tokens(), tokens);
      assert.deepEqual(result.difference(result).tokens(), []);
    }
  });
}, 30000);

test('Polyline.nearlyCovers does not accumulate native clones', () => {
  const { growth } = JSON.parse(runWithGC(async root => {
    const assert = require('node:assert/strict');
    const s2 = require(root);
    const line = new s2.Polyline([
      new s2.LatLng(-40, -40), new s2.LatLng(-40, -39),
    ]);
    const other = new s2.Polyline(Array.from({ length: 1000 }, (_, i) =>
      new s2.LatLng(40, i / 10000)));
    function compare(count) {
      for (let i = 0; i < count; i++) {
        assert.equal(line.nearlyCovers(other, 1e-5), false);
      }
    }
    compare(200); // Warm up native and JS allocation paths before measuring.
    global.gc();
    const before = process.memoryUsage().rss;
    for (let batch = 0; batch < 8; batch++) {
      compare(1000);
      await new Promise(resolve => setImmediate(resolve));
      global.gc();
    }
    console.log(JSON.stringify({ growth: process.memoryUsage().rss - before }));
  }));

  // The old leak retains at least 8000 * 1000 * 24 bytes (~183 MiB).
  // Leave ample headroom for allocator/platform noise; heapUsed alone misses it.
  expect(growth).toBeLessThan(64 * 1024 * 1024);
}, 30000);

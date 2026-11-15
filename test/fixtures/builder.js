const assert = require('node:assert/strict');
// Cover both local builds and the prebuilds produced by npm run build.
const s2 = require('../..');

const outer = [[-32.364, 153.207], [-35.364, 153.207], [-35.364, 158.207], [-32.364, 158.207]];
const hole = [[-33.364, 154.207], [-34.364, 154.207], [-34.364, 155.207], [-33.364, 155.207]].reverse();
const separate = [[-32.364, 160.207], [-35.364, 160.207], [-35.364, 165.207], [-32.364, 165.207]];

// --- Fixture helpers. ---
function loop(ring) {
  return new s2.Loop(ring.map(([lat, lng]) => new s2.LatLng(lat, lng)));
}
// --- End fixture helpers. ---

const builder = new s2.Builder();
switch (process.argv[2]) {
  case 'single':
  case 'hole':
  case 'disjoint': {
    const rings = { single: [outer], hole: [outer, hole], disjoint: [outer, separate] };
    for (const ring of rings[process.argv[2]]) {
      assert.equal(builder.addLoop(loop(ring)), null);
    }
    assert.ok(builder.build() instanceof s2.Polygon);
    break;
  }
  case 'reuse': {
    builder.addLoop(loop(outer));
    const first = builder.build();
    const repeated = builder.build();
    assert.ok(first instanceof s2.Polygon);
    assert.ok(repeated instanceof s2.Polygon);
    assert.notEqual(first, repeated);

    builder.addLoop(loop(separate));
    const second = builder.build();
    assert.ok(second instanceof s2.Polygon);
    assert.notEqual(first, second);
    assert.ok(builder.build() instanceof s2.Polygon);
    break;
  }
  case 'arguments': {
    const noLoop = { name: 'Error', message: 'Add a loop first.' };
    const badLoop = { name: 'TypeError', message: '(loop: s2.Loop) expected.' };
    assert.throws(() => builder.build(), noLoop);
    assert.throws(() => builder.build(), noLoop);
    assert.throws(() => builder.addLoop(), badLoop);
    for (const value of [undefined, null, 0, 'loop', {}, [], new s2.LatLng(0, 0)]) {
      assert.throws(() => builder.addLoop(value), badLoop);
    }
    assert.throws(() => new s2.Loop([]), {
      name: 'RangeError', message: '(latlngs: s2.LatLng[]) was empty.',
    });
    assert.throws(() => builder.build(), noLoop);

    builder.addLoop(loop(outer));
    assert.throws(() => builder.addLoop({}), badLoop);
    assert.ok(builder.build() instanceof s2.Polygon);
    assert.throws(() => builder.addLoop([]), badLoop);
    assert.ok(builder.build() instanceof s2.Polygon);
    builder.addLoop(loop(separate));
    assert.ok(builder.build() instanceof s2.Polygon);
    break;
  }
  case 'failure': {
    const crossing = [[-33, 156], [-36, 156], [-36, 161], [-33, 161]];
    builder.addLoop(loop(outer));
    builder.addLoop(loop(crossing));
    const invalidOutput = { name: 'Error', message: 'Output is invalid.' };
    assert.throws(() => builder.build(), invalidOutput);
    assert.throws(() => builder.build(), invalidOutput);

    builder.addLoop(loop(outer));
    builder.addLoop(loop(hole));
    assert.ok(builder.build() instanceof s2.Polygon);
    assert.ok(builder.build() instanceof s2.Polygon);
    break;
  }
  default:
    throw new Error('Unknown Builder scenario');
}

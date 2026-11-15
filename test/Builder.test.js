const { spawnSync } = require('node:child_process');
const path = require('node:path');

// Keep native failures in a child so a regression reports a failed test.
test.each(['single', 'hole', 'disjoint', 'reuse', 'arguments', 'failure'])(
  'Builder supports %s', (scenario) => {
    const result = spawnSync(process.execPath, [
      path.join(__dirname, 'fixtures/builder.js'), scenario,
    ], { encoding: 'utf8', timeout: 10000 });

    expect(result.error).toBeUndefined();
    expect({ status: result.status, signal: result.signal, stderr: result.stderr })
      .toEqual({ status: 0, signal: null, stderr: '' });
  }
);

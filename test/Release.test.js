const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');

const { npmDistTag } = require('../scripts/npm-dist-tag');
const {
  expectedPrebuilds,
  missingPrebuilds,
  verifyPrebuilds,
} = require('../scripts/verify-prebuilds');

test.each([
  ['0.0.9', 'latest'],
  ['0.0.9-alpha', 'alpha'],
  ['0.0.9-alpha.1', 'alpha'],
  ['1.0.0-rc.2', 'rc'],
])('npmDistTag maps %s to %s', (version, expected) => {
  expect(npmDistTag(version)).toBe(expected);
});

test('verifyPrebuilds rejects incomplete packages', () => {
  const packageRoot = fs.mkdtempSync(path.join(os.tmpdir(), 's2-package-'));

  try {
    expect(missingPrebuilds(packageRoot)).toEqual(expectedPrebuilds);
    expect(() => verifyPrebuilds(packageRoot)).toThrow('Refusing to publish without prebuilds');
  } finally {
    fs.rmSync(packageRoot, { recursive: true });
  }
});

test('verifyPrebuilds accepts packages with every supported binary', () => {
  const packageRoot = fs.mkdtempSync(path.join(os.tmpdir(), 's2-package-'));

  try {
    for (const relativePath of expectedPrebuilds) {
      const prebuild = path.join(packageRoot, relativePath);
      fs.mkdirSync(path.dirname(prebuild), { recursive: true });
      fs.writeFileSync(prebuild, 'prebuild');
    }

    expect(missingPrebuilds(packageRoot)).toEqual([]);
    expect(() => verifyPrebuilds(packageRoot)).not.toThrow();
  } finally {
    fs.rmSync(packageRoot, { recursive: true });
  }
});

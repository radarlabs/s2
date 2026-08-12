const fs = require('node:fs');
const path = require('node:path');

const expectedPrebuilds = Object.freeze([
  'prebuilds/darwin-arm64/@radarlabs+s2.node',
  'prebuilds/linux-arm64/@radarlabs+s2.node',
  'prebuilds/linux-x64/@radarlabs+s2.node',
]);

function missingPrebuilds(rootDir) {
  return expectedPrebuilds.filter(relativePath => (
    !fs.existsSync(path.join(rootDir, relativePath))
  ));
}

function verifyPrebuilds(rootDir) {
  const missing = missingPrebuilds(rootDir);
  if (missing.length > 0) {
    throw new Error(`Refusing to publish without prebuilds:\n${missing.join('\n')}`);
  }
}

if (require.main === module) {
  try {
    verifyPrebuilds(path.resolve(__dirname, '..'));
  } catch (error) {
    console.error(error.message);
    process.exitCode = 1;
  }
}

module.exports = { expectedPrebuilds, missingPrebuilds, verifyPrebuilds };

// Loads the prebuilt N-API addon shipped in ./prebuilds (built by prebuildify),
// falling back to a local ./build compile for unsupported platforms.
module.exports = require('node-gyp-build')(__dirname);

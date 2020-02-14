const binary = require('node-pre-gyp');
const path = require('path');
const binding_path = binary.find(path.resolve(path.join('./package.json')));
const s2 = require(binding_path);
module.exports = s2;

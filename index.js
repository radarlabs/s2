// magic incantation from step 3 @ https://github.com/mapbox/node-pre-gyp#readme
const binary = require('node-pre-gyp');
const path = require('path');
var binding_path = binary.find(path.resolve(path.join(__dirname,'./package.json')));
const s2 = require(binding_path);

module.exports = s2;

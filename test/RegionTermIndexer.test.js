// magic incantation from step 3 @ https://github.com/mapbox/node-pre-gyp#readme
const binary = require("@mapbox/node-pre-gyp");
const path = require("path");
var binding_path = binary.find(path.resolve("./package.json"));
const s2 = require(binding_path);

test("RegionTermIndexer#getIndexTerms works", () => {
  // const dumbo = [40.7033, -73.9881];
  // const ll = new s2.LatLng(...dumbo);
  const tokens = s2.RegionTermIndexer.getIndexTerms({}, "asd");
  expect(tokens).toBe(["89c25a3", "89c25a5", "89c25bb", "89c25bd"]);
});

test("RegionTermIndexer#getQueryTerms works", () => {
  // const dumbo = [40.7033, -73.9881];
  // const ll = new s2.LatLng(...dumbo);
  const tokens = s2.RegionTermIndexer.getQueryTerms({}, "asd");
  expect(tokens).toBe(["89c25a3", "89c25a5", "89c25bb", "89c25bd"]);
});

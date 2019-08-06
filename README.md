Node.js JavaScript & TypeScript bindings for [Google S2](http://s2geometry.io/).

## What is S2?

S2 is a library from Google for easily storing, indexing, and retrieving geographic locations.

Geographic regions can be indexed by S2 cell ids of various levels in a data store and then later retrieved by these ids for extremely quick geolocation lookups.

## The Library

The goal of this library is to maintain node.js TypeScript bindings for the latest version of [Google's C++ S2 library](https://github.com/google/s2geometry).

Other JavaScript projects are unmaintained or are implemented in pure JavaScript.

The project has been built against node's N-API, meaning that it's compatible across node.js versions.

As of today, the library is built and tested run against node.js 8, 10 and 12. The library has been in production use at [Radar](radar.io) and has been built against OS X and Linux. Feel free to open an issue or PR if you'd like other platform support.

See [test.sh](https://github.com/radarlabs/s2/blob/master/test.sh) for more details.

## Usage

To install:

```
npm install @radarlabs/s2
```

To run tests (you'll need Docker):
```
./test.sh
```

To generate a covering for a given area:

```
const s2 = require('@radarlabs/s2');

# an array of lat/lng pairs
const loopLLs = [[40.70113825399865,-73.99229764938354],[40.70113825399865,-73.98766279220581],[40.70382234072197,-73.98766279220581],[40.70382234072197,-73.99229764938354]];

# map to an array of normalized s2.LatLng
const s2LLs = loopLLs.map(([lat, lng]) => (new s2.LatLng(lat, lng)).normalized());

# create a loop
const loop = new s2.Loop(s2LLs);

# pass the loop in to a builder to generate an s2.Polygon
const builder = new s2.Builder();
builder.addLoop(loop);
const polygon = builder.build();

# generate s2 cells to cover this polygon
const coverer = new s2.RegionCoverer({ min: 14, max: 14 });
const covering = coverer.getCovering(polygon);
covering.forEach(c => console.log(c.token()));

> 89c25a31
> 89c25a33
> 89c25a35
> 89c25a37
```

Check if a cell is contained in another:

```
const c1 = s2.CellId.fromToken('89c25a37')
const c2 = s2.CellId.fromToken('89c25')
c2.contains(c1)
> true
c1.contains(c2)
> false
```

For the most part, the bindings are 1:1 with the C++ classes available in Google's S2 Library. If you'd like to see other functions, feel free to open an issue or create a pull request.

More detailed usage can be found in the [tests](https://github.com/radarlabs/s2/tree/master/test) folder.

## Resources

- [A detailed explanation on the S2 library](http://s2geometry.io/)
- [Sidewalk Lab's S2 map for visualizing S2 tokens](https://s2.sidewalklabs.com/regioncoverer/)

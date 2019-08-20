[![CircleCI](https://circleci.com/gh/radarlabs/s2.svg?style=svg&circle-token=ed5b9fcba959e9b786eb5e8d714f9715253e020c)](https://circleci.com/gh/radarlabs/s2)

Node.js JavaScript & TypeScript bindings for [Google S2](http://s2geometry.io/).

## What is S2?

S2 is a library from Google for easily storing, indexing, and retrieving geographic locations.

Geographic regions can be indexed by S2 cell ids of various levels in a data store and then later retrieved by these ids for extremely quick geolocation lookups.

## The Library

The goal of this library is to maintain Node.js TypeScript bindings for the latest version of [Google's C++ S2 library](https://github.com/google/s2geometry).

Other JavaScript projects available on GitHub appear unmaintained.

The project has been built against Node's N-API, meaning that it's compatible across Node.js versions that support BigInt.
This means that Node.js version 9 and below are unsupported.

As of today, the library is built and tested against Node.js 10-12. The library has been in production use at [Radar](https://radar.io) and has been built against OS X and Linux. Feel free to open an issue or PR if you'd like other platform support.

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

# an array of lat/lng pairs representing a region (a part of Brooklyn, in this case)
const loopLLs = [[40.70113825399865,-73.99229764938354],[40.70113825399865,-73.98766279220581],[40.70382234072197,-73.98766279220581],[40.70382234072197,-73.99229764938354]];

# map to an array of normalized s2.LatLng
const s2LLs = loopLLs.map(([lat, lng]) => (new s2.LatLng(lat, lng)));

# generate s2 cells to cover this polygon
const s2level = 14;
const covering = s2.RegionCoverer.getCoveringTokens(s2LLs, { min: s2level, max: s2level });
covering.forEach(c => console.log(c));

> 89c25a31
> 89c25a33
> 89c25a35
> 89c25a37

# check if a point is contained inside this region
const point = new s2.CellId(new s2.LatLng(40.70248844447621, -73.98991584777832));
const pointAtLevel14 = point.parent(s2Level);
console.log(pointAtLevel14.token());
> 89c25a31

const coveringSet = new Set(covering);
console.log(coveringSet.contains(pointAtLevel14.token()));
> true

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

 If you'd like to see more functionality, feel free to open an issue or create a pull request.

More detailed usage can be found in the [tests](https://github.com/radarlabs/s2/tree/master/test) folder.

## Resources

- [A detailed explanation on the S2 library](http://s2geometry.io/)
- [Sidewalk Lab's S2 map for visualizing S2 tokens](https://s2.sidewalklabs.com/regioncoverer/)

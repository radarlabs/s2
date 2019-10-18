#include "builder.h"
#include "cell.h"
#include "cell_id.h"
#include "cell_union.h"
#include "latlng.h"
#include "loop.h"
#include "polygon.h"
#include "region_coverer.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  Builder::Init(env, exports);
  Cell::Init(env, exports);
  CellId::Init(env, exports);
  LatLng::Init(env, exports);
  Loop::Init(env, exports);
  Polygon::Init(env, exports);
  CellUnion::Init(env, exports);
  return RegionCoverer::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)

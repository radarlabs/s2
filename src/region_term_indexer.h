#ifndef RADAR_REGION_TERM_INDEXER
#define RADAR_REGION_TERM_INDEXER

#include <napi.h>
#include "polygon.h"
#include "cell_id.h"
#include "cell_union.h"
#include "s2/s1angle.h"
#include "s2/s2builder.h"
#include "s2/s2builderutil_s2polygon_layer.h"
#include "s2/s2cap.h"
#include "s2/s2cell_union.h"
#include "s2/s2earth.h"
#include "s2/s2polygon.h"
#include "s2/s2region_term_indexer.h"
#include "s2/third_party/absl/memory/memory.h"

class RegionTermIndexer : public Napi::ObjectWrap<RegionTermIndexer>
{

public:
  static Napi::FunctionReference constructor;
  static Napi::Object Init(Napi::Env env, Napi::Object exports);

  RegionTermIndexer(const Napi::CallbackInfo &info);

private:
  static Napi::Value GetIndexTerms(const Napi::CallbackInfo &info);
  static Napi::Value GetQueryTerms(const Napi::CallbackInfo &info);
};

#endif

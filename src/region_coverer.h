#ifndef RADAR_REGION_COVERER
#define RADAR_REGION_COVERER

#include <napi.h>
#include "polygon.h"
#include "cell_id.h"
#include "s2/s2builder.h"
#include "s2/s2region_coverer.h"
#include "s2/s2builderutil_s2polygon_layer.h"
#include "s2/s2polygon.h"
#include "s2/third_party/absl/memory/memory.h"

class RegionCoverer : public Napi::ObjectWrap<RegionCoverer> {

  public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    RegionCoverer(const Napi::CallbackInfo& info);

  private:
    static Napi::Value GetCoveringTokens(const Napi::CallbackInfo &info);
};

#endif

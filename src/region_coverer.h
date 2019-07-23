#ifndef RADAR_REGION_COVERER
#define RADAR_REGION_COVERER

#include <napi.h>
#include "polygon.h"
#include "cell_id.h"
#include "s2/s2region_coverer.h"

class RegionCoverer : public Napi::ObjectWrap<RegionCoverer> {

  public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    RegionCoverer(const Napi::CallbackInfo& info);

  private:
    Napi::Value GetCovering(const Napi::CallbackInfo &info);

    std::unique_ptr<S2RegionCoverer::Options> options;
    std::unique_ptr<S2RegionCoverer> s2regioncoverer;
};

#endif

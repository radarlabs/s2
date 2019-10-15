#ifndef RADAR_REGION_COVERER
#define RADAR_REGION_COVERER

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
#include "s2/s2region_coverer.h"
#include "s2/third_party/absl/memory/memory.h"

class RegionCoverer : public Napi::ObjectWrap<RegionCoverer> {

  public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    RegionCoverer(const Napi::CallbackInfo& info);

  private:
    static Napi::Value GetCoveringIds(const Napi::CallbackInfo &info);
    static Napi::Value GetCoveringTokens(const Napi::CallbackInfo &info);
    static Napi::Value GetCoveringCellUnion(const Napi::CallbackInfo &info);

    static Napi::Value GetRadiusCoveringIds(const Napi::CallbackInfo &info);
    static Napi::Value GetRadiusCoveringTokens(const Napi::CallbackInfo &info);
    static Napi::Value GetRadiusCoveringCellUnion(const Napi::CallbackInfo &info);

    static void GetS2Options(
      Napi::Object optionsObject,
      S2RegionCoverer::Options &options
    );

    static S2CellUnion GetCovering(
      Napi::Env env,
      std::vector<S2Point> &vertices,
      S2RegionCoverer::Options &coverOptions,
      S2Error &loopError,
      S2Error &buildError,
      S2Error &outputError
    );

    static S2CellUnion GetRadiusCovering(
      LatLng* ll,
      double radiusM,
      S2RegionCoverer::Options &coverOptions
    );
};

#endif

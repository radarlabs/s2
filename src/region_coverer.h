#ifndef RADAR_REGION_COVERER
#define RADAR_REGION_COVERER

#include <napi.h>
#include <memory>
#include "s2/base/stringprintf.h"
#include "s2/s2error.h"
#include "s2/s2loop.h"
#include "cell_id.h"
#include "cell_union.h"
#include "s2/s1angle.h"
#include "s2/s2cap.h"
#include "s2/s2cell_union.h"
#include "s2/s2earth.h"
#include "s2/s2polygon.h"
#include "s2/s2region_coverer.h"

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
      const std::vector<S2Point> &vertices,
      const S2RegionCoverer::Options &coverOptions,
      S2Error &loopError
    );

    static S2CellUnion GetRadiusCovering(
      LatLng* ll,
      double radiusM,
      S2RegionCoverer::Options &coverOptions
    );
};

#endif

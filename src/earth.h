#ifndef RADAR_EARTH
#define RADAR_EARTH

#include <napi.h>
#include "latlng.h"
#include "s2/s1angle.h"
#include "s2/s2latlng.h"
#include "s2/s2earth.h"

class Earth : public Napi::ObjectWrap<Earth> {
  public:
    Earth(const Napi::CallbackInfo& info);

    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    S2Earth Get();

  private:
    static Napi::Value ToMeters(const Napi::CallbackInfo &info);
    static Napi::Value ToKm(const Napi::CallbackInfo &info);
    static Napi::Value GetRadians(const Napi::CallbackInfo &info);
    static Napi::Value GetDegrees(const Napi::CallbackInfo &info);
    static Napi::Value GetDistanceKm(const Napi::CallbackInfo &info);
    static Napi::Value GetDistanceMeters(const Napi::CallbackInfo &info);
    static Napi::Value GetInitalBearingDegrees(const Napi::CallbackInfo &info);

    static bool PreCheck(const Napi::CallbackInfo &info, S2LatLng &latlng1, S2LatLng &latlng2);

    S2Earth s2earth;
};

#endif

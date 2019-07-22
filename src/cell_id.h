#ifndef RADAR_CELLID
#define RADAR_CELLID

#include <napi.h>
#include <sstream>

#include "latlng.h"
#include "s2/s2cell_id.h"
#include "s2/util/coding/varint.h"

class CellId : public Napi::ObjectWrap<CellId> {

  public:
    CellId(const Napi::CallbackInfo& info);

    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    S2CellId Get();

  private:
    static Napi::Value FromToken(const Napi::CallbackInfo &info);
    static Napi::Value FromTokenString(const Napi::Env env, const string token);

    Napi::Value Id(const Napi::CallbackInfo &info);
    Napi::Value Token(const Napi::CallbackInfo &info);

    Napi::Value Contains(const Napi::CallbackInfo &info);
    Napi::Value Intersects(const Napi::CallbackInfo &info);

    Napi::Value Parent(const Napi::CallbackInfo &info);
    Napi::Value Child(const Napi::CallbackInfo &info);
    Napi::Value Level(const Napi::CallbackInfo &info);
    Napi::Value IsLeaf(const Napi::CallbackInfo &info);

    S2CellId s2cellid;

};

#endif

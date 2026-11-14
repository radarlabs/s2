#include "region_term_indexer.h"

using absl::make_unique;
using absl::string_view;
using std::vector;

Napi::FunctionReference RegionTermIndexer::constructor;

Napi::Object RegionTermIndexer::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "RegionTermIndexer", {
                                                                  StaticMethod("getIndexTerms", &RegionTermIndexer::GetIndexTerms),
                                                                  StaticMethod("getQueryTerms", &RegionTermIndexer::GetQueryTerms),
                                                              });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("RegionTermIndexer", func);
  return exports;
}

RegionTermIndexer::RegionTermIndexer(const Napi::CallbackInfo &info) : Napi::ObjectWrap<RegionTermIndexer>(info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
}

Napi::Value RegionTermIndexer::GetIndexTerms(const Napi::CallbackInfo &info)
{
  // TODO implement properly.
  Napi::Env env = info.Env();

  uint32_t size = 5;
  Napi::Array returnedIds = Napi::Array::New(env, size);

  for (uint32_t i = 0; i < size; i++)
  {
    returnedIds[i] = i;
  }

  return returnedIds;

  // Napi::Env env = info.Env();

  // S2RegionTermIndexer termIndexer;
  // S2Point point(0.1, -0.4, 0.3);
  // vector<string> result = termIndexer.GetIndexTerms(point, string_view("foo"));

  // uint32_t size = 5;
  // Napi::Array returnedIds = Napi::Array::New(env, size);

  // for (uint32_t i = 0; i < size; i++)
  // {
  //   returnedIds[i] = result[i];
  // }

  // return returnedIds;
}

Napi::Value RegionTermIndexer::GetQueryTerms(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  S2RegionTermIndexer::Options options;
  S2RegionTermIndexer termIndexer(options);

  S2Point point(0.1, -0.4, 0.3);
  absl::string_view str("foo");

  // This line causes it to die at runtime with the error:
  // dyld[15800]: missing symbol called
  // Abort trap: 6
  std::vector<std::string> result = termIndexer.GetQueryTerms(point, str);

  // TODO this is just so we're returning the correct type for now. WIP.
  uint32_t size = 5;
  Napi::Array returnedIds = Napi::Array::New(env, size);

  for (uint32_t i = 0; i < size; i++)
  {
    returnedIds[i] = i;
  }

  return returnedIds;
}
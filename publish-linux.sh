#!/bin/bash

set -ex

for platform in linux/amd64 linux/arm64
do
  for node in 20.0.0 22.0.0 24.0.0
  do
    # run image
    docker run -it \
      --platform "$platform" \
      -v "$PWD":/app \
      -e AWS_DEFAULT_REGION \
      -e AWS_ACCESS_KEY_ID \
      -e AWS_SECRET_ACCESS_KEY \
      -e AWS_SESSION_TOKEN \
      node:$node \
      bash -c "
        set -ex
        cd /app
        rm -rf build node_modules lib/binding/Release
        npm ci
        JOBS=max npx --no-install node-pre-gyp rebuild package unpublish publish
      "
  done
done

#!/bin/bash

set -ex

export DOCKER_DEFAULT_PLATFORM="${DOCKER_DEFAULT_PLATFORM:-linux/amd64}"

for node in 20.0.0 18.12.1 16.5.0
do
  # run image
  docker run -it \
    -v "$PWD":/app \
    -e AWS_DEFAULT_REGION \
    -e AWS_ACCESS_KEY_ID \
    -e AWS_SECRET_ACCESS_KEY \
    -e AWS_SESSION_TOKEN \
    node:$node \
    bash -c 'cd /app && npm install && JOBS=max npx node-pre-gyp build package unpublish publish'
done
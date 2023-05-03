#!/bin/bash

set -ex

git submodule init
git submodule sync
git submodule update

for node in 16 18 20
do
  # build image
  docker build -t test$node -f ./docker/Dockerfile.node$node.test .

  # run image
  docker run test$node:latest
done
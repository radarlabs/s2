#!/bin/bash

set -ex

git submodule init
git submodule sync
git submodule update

# loop through node 14, 16, 18 and test
for node in 14 16 18
do
  # build image
  docker build -t test$node -f ./docker/Dockerfile.node$node.test .

  # run image
  docker run test$node:latest
done
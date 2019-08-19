#!/bin/bash

set -ex

git submodule init
git submodule update --remote


# test node 10
NODE_10_IMAGE=test10
NODE_10_DOCKERFILE=./docker/Dockerfile.node10.test
docker build -t $NODE_10_IMAGE -f ./docker/Dockerfile.node10.test . && docker run $NODE_10_IMAGE:latest

# test node 11
NODE_11_IMAGE=test11
NODE_11_DOCKERFILE=./docker/Dockerfile.node11.test
docker build -t $NODE_11_IMAGE -f $NODE_11_DOCKERFILE . && docker run $NODE_11_IMAGE:latest

# test node 12
NODE_12_IMAGE=test12
NODE_12_DOCKERFILE=./docker/Dockerfile.node12.test
docker build -t $NODE_12_IMAGE -f $NODE_12_DOCKERFILE . && docker run $NODE_12_IMAGE:latest

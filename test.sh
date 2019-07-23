#!/bin/bash

set -ex

# test node 8
NODE_8_IMAGE=test8
NODE_8_DOCKERFILE=./docker/Dockerfile.node8.test
docker build -t $NODE_8_IMAGE -f $NODE_8_DOCKERFILE . && docker run $NODE_8_IMAGE:latest

# test node 10
NODE_10_IMAGE=test10
NODE_10_DOCKERFILE=./docker/Dockerfile.node10.test
docker build -t $NODE_10_IMAGE -f ./docker/Dockerfile.node10.test . && docker run $NODE_10_IMAGE:latest


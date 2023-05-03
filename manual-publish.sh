#!/bin/bash

# this script is mainly for running on macs to publish binaries
# for linux, we use a circleci/docker build
#
# per the node-gyp-pre docs, you can use any of the standard ways
# to specify AWS keys from the javascript aws-sdk, or the env vars
# node_pre_gyp_accessKeyId and node_pre_gyp_secretAccessKey

set -e

# loop through node LTS versions 16 - 20, unpublish and publish them
for node in v16 v18 v20
do
  nvm install $node
  nvm use $node
  rm -rf node_modules
  npm install
  JOBS=max npx node-pre-gyp rebuild package unpublish publish
done
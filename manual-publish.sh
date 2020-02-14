#!/bin/sh

# this script is mainly for running on macs to publish binaries
# for linux, we use a circleci/docker build

set -e

export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"  # This loads nvm

nvm install v10
nvm use v10
rm -rf node_modules
JOBS=max npx node-pre-gyp rebuild package unpublish publish

nvm install v11
nvm use v11
rm -rf node_modules
JOBS=max npx node-pre-gyp rebuild package unpublish publish

nvm install v12
nvm use v12
rm -rf node_modules
JOBS=max npx node-pre-gyp rebuild package unpublish publish

nvm install v13
nvm use v13
rm -rf node_modules
JOBS=max npx node-pre-gyp rebuild package unpublish publish

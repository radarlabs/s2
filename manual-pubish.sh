#!/bin/sh

set -e

export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"  # This loads nvm

nvm install v10
nvm use v10
rm -rf node_modules
npm install
npx node-pre-gyp rebuild package unpublish publish

nvm install v11
nvm use v11
rm -rf node_modules
npm install
npx node-pre-gyp rebuild package unpublish publish

nvm install v12
nvm use v12
rm -rf node_modules
npm install
npx node-pre-gyp rebuild package unpublish publish

nvm install v13
nvm use v13
rm -rf node_modules
npm install
npx node-pre-gyp rebuild package unpublish publish

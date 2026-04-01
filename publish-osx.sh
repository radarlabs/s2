#!/bin/zsh

# to publish x86_64, run with arch -x86_64

set -e

export NVM_DIR="$HOME/.nvm"
source "$NVM_DIR/nvm.sh"

# loop through node LTS versions 20 - 24, unpublish and publish them
for node in v20 v22 v24
do
  nvm install $node
  nvm use $node
  echo "building darwin-$(node -p 'process.arch')"
  rm -rf build node_modules lib/binding/Release
  npm ci
  JOBS=max npx --no-install node-pre-gyp rebuild package unpublish publish
done

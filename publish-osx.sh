#!/bin/zsh

# to publish x86_64, run with arch -x86_64

set -e

# source ~/.zshrc

# loop through node LTS versions 20 - 24, unpublish and publish them
for node in v20 v22 v24
do
  nvm install $node
  nvm use $node
  rm -rf node_modules
  npm ci
  JOBS=max npx --no-install node-pre-gyp build package unpublish publish
done

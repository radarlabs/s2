const { version } = require('../package.json');

function npmDistTag(packageVersion) {
  const prerelease = packageVersion.split('-', 2)[1];
  return prerelease ? prerelease.split('.', 1)[0] : 'latest';
}

if (require.main === module) {
  process.stdout.write(npmDistTag(version));
}

module.exports = { npmDistTag };

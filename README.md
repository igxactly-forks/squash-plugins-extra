This repository contains plugins for
[Squash](https://quixdb.github.io/squash) which are not appropriate
for distributions alongside Squash, but may still be useful.

Issues which prevent a plugin from being distributed with Squash
include:

 * Unfixed security vulnerabilities.
 * Non-portable code.
 * Using proprietary libraries.

# Plugins

Plugins currently included in this repository are:

| Plugin | Library | Reason not distributed with Squash |
| ------ | ------- | ---------------------------------- |
| pithy  | [Pithy](https://github.com/johnezang/pithy) | Security issues (see [CompFuzz results](https://github.com/nemequ/compfuzz/wiki/Results)) |

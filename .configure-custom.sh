#!/bin/sh

DISABLE_VARS="external|yes|FORCE_IN_TREE_DEPENDENCIES"
DISABLE_FORCE_IN_TREE_DEPENDENCIES_DOC="force in-tree dependencies, even when a system library is available"

for plugin in \
    pithy \
    lzsse; do
  NAME_UC=$(echo $plugin | tr '[:lower:]' '[:upper:]' | sed 's/[^A-Z0-9]/_/g')
  DISABLE_VARS="${DISABLE_VARS} ${plugin}|no|ENABLE_${NAME_UC}"
  eval "DISABLE_ENABLE_${NAME_UC}_DOC=\"disable the ${plugin} plugin\""
done

ENABLE_VARS="undefined|yes|ALLOW_UNDEFINED installed-tests|yes|ENABLE_INSTALLED_TESTS"
ENABLE_ALLOW_UNDEFINED_DOC="allow plugins to leverage undefined behavior"
ENABLE_ENABLE_INSTALLED_TESTS_DOC="install unit tests"

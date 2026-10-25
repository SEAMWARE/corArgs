#!/bin/bash
#
# FILE            parseTest.sh
#
# AUTHOR          Ken Zangelin
#
# Copyright 2026 Seamware
# SPDX-License-Identifier: Apache-2.0
#
# corArgsParse on what a command line and the environment can hold (test/parseTest.c is the program):
#   - a required option missing: named, the usage, exit 1; given on the command line or in the environment: fine
#   - --usage/-u and --Usage/-U print the list whatever else is wrong on the command line (`-u -p 0`)
#   - the action options (--usage, --Usage, --version) have no environment variable: not listed, not read
#
#   parseTest.sh <path to parseTest>
#
P=${1:?usage: parseTest.sh <path to parseTest>}
failures=0

#
# check <description> <expected exit code> <grep -E pattern the output must match, or !pattern it must not> <command...>
#
check() {
  local what=$1 rcWanted=$2 pattern=$3
  shift 3

  local out rc
  out=$("$@" 2>&1)
  rc=$?

  local ok=1
  [ "$rc" = "$rcWanted" ] || ok=0
  if [ "${pattern:0:1}" = '!' ]; then
    command grep -q -E -- "${pattern:1}" <<< "$out" && ok=0
  else
    command grep -q -E -- "$pattern" <<< "$out" || ok=0
  fi

  if [ $ok = 1 ]; then
    echo "OK      $what"
  else
    echo "FAILED  $what (exit code $rc, expected $rcWanted; output must match '$pattern'):"
    sed 's/^/          /' <<< "$out"
    failures=$((failures + 1))
  fi
}

unset PARSETEST_NAME PARSETEST_PORT PARSETEST_VERSION PARSETEST_USAGE

check "required option missing: named"                 1 '^parseTest: missing required option: --name$'  "$P"
check "required option missing: the usage follows"     1 '^Usage: parseTest'                              "$P"
check "required option on the command line"            0 '^OK name x port 7900 version false$'            "$P" --name x
check "required option, short name"                    0 '^OK name x port 7900 version false$'            "$P" -n x
check "required option in the environment"             0 '^OK name y port 7900 version false$'            env PARSETEST_NAME=y "$P"
check "required option missing, --version given"       1 '^parseTest: missing required option: --name$'  "$P" --version
check "-u with a bad value of another option"          0 '^Usage: parseTest'                              "$P" -u -p 0
check "-u after a bad value of another option"         0 '^Usage: parseTest'                              "$P" -p 0 -u
check "-u: no range error printed"                     0 '!out of range|lower limit'                      "$P" -u -p 0
check "-U with a bad value of another option"          0 '--name'                                         "$P" -p 0 -U
check "-u with a required option missing"              0 '^Usage: parseTest'                              "$P" -u
check "-u inside a group of bool options (-Vu)"        0 '^Usage: parseTest'                              "$P" -Vu -p 0
check "-u as the VALUE of --name is a name"            0 '^OK name -u port 7900 version false$'           "$P" --name -u
check "a bad value without -u is still an error"       3 'lower limit'                                    "$P" -p 0 --name x
check "-U lists the environment variable of --name"    0 'PARSETEST_NAME'                                 "$P" -U
check "-U lists no variable for --version"             0 '!PARSETEST_VERSION'                             "$P" -U
check "-U lists no variable for --usage, --Usage"      0 '!PARSETEST_USAGE'                               "$P" -U
check "PARSETEST_VERSION is not read"                  0 '^OK name x port 7900 version false$'            env PARSETEST_VERSION=true "$P" --name x
check "PARSETEST_USAGE is not read"                    0 '^OK name x port 7900 version false$'            env PARSETEST_USAGE=true "$P" --name x

if [ $failures != 0 ]; then
  echo "parseTest: $failures FAILED"
  exit 1
fi

echo "parseTest: all OK"

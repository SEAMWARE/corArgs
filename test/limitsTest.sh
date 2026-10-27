#!/bin/bash
#
# FILE            limitsTest.sh
#
# AUTHOR          Ken Zangelin
#
# Copyright 2026 Seamware
# SPDX-License-Identifier: Apache-2.0
#
# An option's limits and corArgsInit's check of the table (test/limitsTest.c is the program):
#   - a string's limits are strings, compared with strcmp: below the min and after the max refused, the limit named
#   - a float's limits are checked as an integer's are
#   - a default value outside an option's only limit (a max and no min): the table refused, the option named
#   - no default (CORARGS_NL) with a limit: fine
#   - two options with one name: the table refused, the option named
#   - a section heading (CORARGS_SEPARATOR) in the table given to corArgsInit: fine
#
#   limitsTest.sh <path to limitsTest>
#
P=${1:?usage: limitsTest.sh <path to limitsTest>}
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

unset LIMITSTEST_STR LIMITSTEST_RATE LIMITSTEST_COUNT

check "a separator in the table, the defaults"     0 '^OK str 060 rate 2$'                                  "$P" limits
check "a string inside its limits"                 0 '^OK str 07 rate 2$'                                   "$P" limits --str 07
check "a string equal to its max"                  0 '^OK str 100 rate 2$'                                  "$P" limits --str 100
check "a string below its min: refused"            3 "value '04' for option '--str' is inferior"            "$P" limits --str 04
check "a string after its max: refused"            3 "value '2' for option '--str' exceeds its upper limit" "$P" limits --str 2
check "a string after its max: the range named"    3 "Allowed range: '050' - '100'"                         "$P" limits --str 2
check "a string after its max, from the env"       3 "exceeds its upper limit"                              env LIMITSTEST_STR=2 "$P" limits
check "a float inside its limits"                  0 '^OK str 060 rate 5.5$'                                "$P" limits --rate 5.5
check "a float below its min: refused"             3 "value 0.500000 for option '--rate' is inferior"       "$P" limits --rate 0.5
check "a float above its max: refused"             3 "value 11.000000 for option '--rate' exceeds"          "$P" limits --rate 11
check "a default above the only limit: refused"    2 "^corArgsInit: .* option '--count'$"                   "$P" defOverMax
check "no default, a max: fine"                    0 '^OK'                                                  "$P" defNoLimit
check "a name twice: refused, the option named"    2 "^corArgsInit: .* option '--dup'$"                     "$P" dupLong

if [ $failures != 0 ]; then
  echo "limitsTest: $failures FAILED"
  exit 1
fi

echo "limitsTest: all OK"

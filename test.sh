#!/bin/sh

_ROOT_DIR_="`cd -- $(dirname -- $0) && pwd`"
_TEST_="${_TEST_:-basic}"
_RLS_CONFIG_="${_RLS_CONFIG_}"
_RLS_CLEAN_="${_RLS_CLEAN_}"
_OS_NAME_="`uname -s 2>/dev/null`"
_WIN_ENV_=
_WIN_ENV_MSVC_=

case "$_OS_NAME_" in
  MSYS_NT-*|MINGW??_NT-*) _OS_NAME_="WinNT" ;;
esac

CC="${CC}"
if [ -z "$CC" ]; then
  case "$_OS_NAME_" in
    Darwin)  CC="clang" ;;
    Linux)   CC="gcc"   ;;
    WinNT)   CC="cl"    ;;
  esac
fi

# switch to ROOT
cd "${_ROOT_DIR_}"

# check nore
if [ ! -f "${_ROOT_DIR_%/}/configure" ]; then
  curl -O "https://raw.githubusercontent.com/junjiemars/nore/master/bootstrap.sh" -o bootstrap.sh
  if [ 0 -ne $? ]; then
    echo "!panic: install Nore failed"
    exit 1
  fi
  sh bootstrap.sh
  rm bootstrap.sh
fi

# check cc-env for cl
if [ "WinNT" = "${_OS_NAME_}" -a "cl" = "${CC}" ]; then
  if [ ! -f "${HOME}/.nore/cc-env.sh" ]; then
    echo "!panic: ${HOME}/.nore/cc-env.sh no found"
    exit 1
  fi
  ${HOME}/.nore/cc-env.sh 1
  if [ ! -f "${HOME}/.nore/cc-env.bat" ]; then
    echo "!panic: ${HOME}/.nore/cc-env.bat no found"
    exit 1
  fi
  cat << END > "${HOME}/msvc.bat"
@if not "%VSCMD_DEBUG%" GEQ "3" echo off
REM generated by Nore (https://github.com/junjiemars/nore)
call "%1"
sh "%2"
"%3"
END
  if [ ! -f "${HOME}/msvc.bat" ]; then
    echo "!panic: generate ${HOME}/msvc.bat failed"
    exit 1
  fi
  _WIN_ENV_MSVC_="${HOME}/msvc.bat"
  chmod u+x "${_WIN_ENV_MSVC_}"
  _WIN_ENV_="${HOME}/.nore/cc-env.bat"
fi

test_do() {
  local retval=0
  if [ -z "$_WIN_ENV_" ]; then
    ${_ROOT_DIR_%/}/configure "$_RLS_CONFIG_" "$@" && make -C "${_ROOT_DIR_%/}" "$_RLS_CLEAN_" test
  else
    echo "${_WIN_ENV_MSVC_} $_WIN_ENV_ ./configure $_RLS_CONFIG_ $@ make $_RLS_CLEAN_ test"
    ${_WIN_ENV_MSVC_} "$_WIN_ENV_" "./configure $_RLS_CONFIG_ $@" "make $_RLS_CLEAN_ test"
  fi
  retval=$?
  if [ 0 -ne $retval ]; then
    echo "------------"
    echo "! $@ <failed>"
    echo "------------"
  fi
  return $retval
}

# basic test
if [ "basic" = "$_TEST_" ]; then
  test_do --has-hi       || exit 1
  test_do --has-algo     || exit 1
  test_do --has-ds       || exit 1
  test_do --has-library  || exit 1
  test_do --has-lang     || exit 1
  test_do --has-memory   || exit 1
  test_do --has-regexp   || exit 1
  test_do --has-unicode  || exit 1
  test_do --has-x86      || exit 1
  test_do --has-os       || exit 1
fi

# eof

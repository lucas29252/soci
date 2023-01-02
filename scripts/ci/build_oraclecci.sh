#!/usr/bin/env bash
# Builds SOCI Oracle C++ Call Interface backend in CI builds
#
# Copyright (c) 2013 Mateusz Loskot <mateusz@loskot.net>
#
source ${SOCI_SOURCE_DIR}/scripts/ci/common.sh

cmake ${SOCI_DEFAULT_CMAKE_OPTIONS} \
    -DWITH_BOOST=OFF \
    -DSOCI_ORACLE=ON \
    -DSOCI_ORACLECCI=ON \
    -DSOCI_ORACLECCI_TEST_CONNSTR:STRING="service=localhost/XE user=travis password=travis" \
    ..

run_make

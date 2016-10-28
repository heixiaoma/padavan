#!/bin/sh

CONTAINER=padavan_build

docker stop -t 0 "$CONTAINER" && docker rm -fv "$CONTAINER"

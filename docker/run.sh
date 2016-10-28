#!/bin/sh

PRJROOT=`readlink -e \`dirname $0\`/../`
IMAGE_TAG=padavan:env
CONTAINER=padavan_build

docker run -id --name="$CONTAINER" \
-v "$PRJROOT:/padavan" \
"$IMAGE_TAG"

#!/bin/sh

CONTAINER=padavan_build

docker exec -it "$CONTAINER" bash -c "cd /padavan && bash"

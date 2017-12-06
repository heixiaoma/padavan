#!/usr/bin/env bash

ImageName=padavan:rtn18u
HostName=padavan
SrcDir=""

pushd $(dirname $0)/../ && SrcDir=$(pwd) && popd || { echo "Fail to retrieve the source directory path."; exit 1; }

[ "$(docker images -q ${ImageName})" == "" -o "$1" == "update" ] && {
	pushd $(dirname $0)
	docker build --build-arg HOME=${HOME} --build-arg USER=${USER} --build-arg UID=${UID} -t ${ImageName} .
	popd
}

docker run --rm --hostname ${HostName} -v ${SrcDir}:${HOME}/$(basename ${SrcDir}) -it ${ImageName}

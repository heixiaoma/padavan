#!/usr/bin/env bash

ImageName=padavan
ImageTag=rtn18u
SrcName=${ImageName}
HostName=${ImageName}
Image=${ImageName}:${ImageTag}

function usage() {
	cat <<EOF >&2
Usage: $(basename $0) [options]

Options:
   -u|--update
      update/rebuild the docker image before run this image
      default: false
   -f|--force
      force rebuild the docker image without docker cache
      default: false
   -h|--help
      show this usage

EOF
}

# options
Update=0
Force=0
Help=0
Opts=$(getopt -o ufh --long update,force,help -- "$@")
[[ $? != 0 ]] && { usage; exit 1; }
eval set -- "$Opts"
while true; do
	case "$1" in
		-u|--update) Update=1; shift;;
		-f|--force)  Update=1; Force=1; shift;;
		-h|--help)   Help=1; shift;;
		--)          shift; break;;
		*) echo "Arguments parsing error"; exit 1;;
	esac
done

[[ "$Help" == 1 ]] && { usage; exit 0; }

[[ "$(docker images -q ${Image})" == "" || "$Update" == 1 ]] && {
	pushd $(dirname $0)
	[[ "$Force" == 1 ]] && buildopts="--no-cache"
	docker build ${buildopts} --build-arg HOME=${HOME} --build-arg USER=${USER} --build-arg UID=${UID} -t ${Image} .
	popd
}

docker run --rm --hostname ${HostName} -v $(pwd):${HOME}/${SrcName} -it ${Image}

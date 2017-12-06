FROM ubuntu:trusty

LABEL maintainer="YiJun Chen <gzenux@gmail.com>"


# install basic required packages
RUN apt-get update && apt-get install -y \
	automake autopoint \
	bison build-essential \
	gawk gettext \
	flex \
	libtool \
	module-init-tools \
	pkg-config python-docutils \
	rsync \
	sudo \
	texinfo \
	unzip \
	wget \
	libgmp3-dev \
	libmpc-dev \
	libmpfr-dev \
	libncurses5-dev \
	zlib1g-dev

# generate locale
RUN locale-gen en_US.UTF-8

# Add user and switch to the user
ARG UID
ARG USER
ARG HOME
RUN useradd -m -u ${UID} -d ${HOME} ${USER} && passwd -d ${USER} && \
gpasswd -a ${USER} sudo && sed -i 's:%sudo\tALL=(ALL\:ALL) ALL:%sudo\tALL=NOPASSWD\: ALL:g' /etc/sudoers

USER ${USER}
WORKDIR ${HOME}

CMD /bin/bash


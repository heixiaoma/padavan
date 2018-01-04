FROM ubuntu:14.04

LABEL maintainer="YiJun Chen <gzenux@gmail.com>"


# install basic packages
RUN apt-get update && apt-get install -y locales sudo

# install required packages
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
ARG UID=1000
ARG USER=user
ARG HOME=/home/${USER}
RUN useradd -m -u ${UID} -d ${HOME} ${USER} && passwd -d ${USER} && \
gpasswd -a ${USER} sudo && sed -i 's:%sudo\tALL=(ALL\:ALL) ALL:%sudo\tALL=NOPASSWD\: ALL:g' /etc/sudoers

USER ${USER}
WORKDIR ${HOME}

CMD ["/bin/bash"]

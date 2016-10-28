FROM ubuntu:trusty

MAINTAINER YiJun Chen <gzenux@gmail.com>


# Essential packages
RUN apt-get update && apt-get install -y \
	git \
	build-essential \
	gawk \
	pkg-config \
	gettext \
	automake \
	autoconf \
	autopoint \
	libtool \
	bison \
	flex \
	zlib1g-dev \
	libgmp3-dev \
	libmpfr-dev \
	libmpc-dev \
	texinfo \
	mc \
	libncurses5-dev \
	nano \
	vim

# Extra packages
RUN apt-get install -y \
	sudo \
	bash-completion

# Get the arguments from command line
ARG toolchain
ARG user
ARG uid

# Build the toolchain
COPY $toolchain /opt/rt-n56u/toolchain-mipsel
RUN cd /opt/rt-n56u/toolchain-mipsel && ./clean_sources && ./build_toolchain_3.4.x

# Create user account
RUN groupadd -g $uid $user && useradd -u $uid -mg $user $user && passwd -d $user && \
	gpasswd -a $user sudo && sed -i 's:%sudo\tALL=(ALL\:ALL) ALL:%sudo\tALL=NOPASSWD\: ALL:g' /etc/sudoers

USER $user

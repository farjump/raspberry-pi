# Development container base image

FROM debian:stretch

RUN apt-get -q update

# lang settings
RUN apt-get -q install -y --no-install-recommends locales && locale-gen C.UTF-8
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

# Dependencies:
# sudo: for privileged execution
# curl: used to download files
# bzip2: tarball uncompression
# ca-certificates: HTTPS downloads
RUN apt-get update && apt-get install --no-install-recommends -y \
  sudo \
  bzip2 \
  curl \
  make \
  ca-certificates

# Official distribution of the ARM Toolchain
COPY ./scripts/ /tmp/scripts/
RUN bash -c /tmp/scripts/2-install-toolchain.sh

# Create a non-root user with the same uid as on the host to allow proper file
# permissions created inside the container. Since it is not root, allow calling
# sudo without password when required.
ARG uid
RUN useradd -m --uid $uid --user-group user \
    && echo 'user ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers.d/user \
    && chmod a=r,o= /etc/sudoers.d/user
USER user

# man bash explains SIGTERM is ignored and that SIGHUP
# stops and dispatches the signal to running childs
STOPSIGNAL SIGHUP
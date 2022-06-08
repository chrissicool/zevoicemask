FROM ubuntu:latest
LABEL maintainer="Christian Ludwig <chrissicool@gmail.com>"

WORKDIR /zevoicemask
ARG UID

RUN apt-get update && apt-get install -y git python3-pip python3-dev
RUN useradd --no-log-init -m -u ${UID:-1000} ci

USER ci
RUN mkdir -p "${HOME}"/.local/bin

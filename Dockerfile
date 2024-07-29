FROM ubuntu:latest
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y gcc make
COPY src /MateusHonorato/src
COPY Makefile /MateusHonorato
RUN cd /MateusHonorato && make all

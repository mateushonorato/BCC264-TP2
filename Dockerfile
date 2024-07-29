FROM alpine:latest
RUN apk add --no-cache gcc make musl-dev
COPY src /MateusHonorato/src
COPY Makefile /MateusHonorato
RUN cd /MateusHonorato && make all

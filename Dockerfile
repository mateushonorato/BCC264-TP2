FROM ubuntu:latest
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y g++ psmisc gcc vim grep
COPY . /MateusHonorato
# RUN make /app
# CMD apt-get update && apt-get install -y g++ psmisc 
## && g++ /MateusHonorato/Programa1.cpp -o /MateusHonorato/Programa1 && ./MateusHonorato/Programa1

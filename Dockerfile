FROM ubuntu:latest

ADD /compiler/gcc-4.6.1 /gcc

ENV PATH "$PATH:/gcc/bin"

RUN apt-get update
RUN apt-get -y install build-essential
RUN apt-get -y install bsdmainutils

FROM ubuntu:latest

ADD /compiler/gcc-4.6.1 /gcc

ENV PATH "$PATH:/gcc/bin"

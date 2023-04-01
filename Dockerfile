FROM ubuntu

RUN apt update && apt install git python3 gcc g++ -y
RUN git clone https://github.com/ctng-dcoe/re-101
WORKDIR exercise
RUN g++ /re-101/decoder.cpp -o malware


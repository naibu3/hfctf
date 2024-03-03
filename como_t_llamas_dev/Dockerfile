FROM debian

RUN apt update
RUN apt install -y \
    ucspi-tcp make gcc gcc-multilib

COPY ./src/ /home/ctf/

WORKDIR /home/ctf/

RUN make
RUN chmod +x run.sh
RUN echo 'hfctf{PU3D3S_114M4RM3_CU4ND0_QU13R4$} > /home/ctf/flag.txt'

EXPOSE 19303

CMD tcpserver -t 50 -RHl0 0.0.0.0 31337 ./run.sh
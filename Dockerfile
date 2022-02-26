FROM debian:latest

RUN mkdir traceroute
COPY . traceroute/.

RUN chmod +x /traceroute/srcs/setup_ssh.sh

RUN apt-get update
RUN apt-get -y install make
RUN apt-get -y install traceroute
RUN apt-get -y install iputils-ping
RUN apt-get -y install gcc

#CMD "while; do sleep 1 done"
CMD sleep 1000000
#ENTRYPOINT ["/traceroute/srcs/setup_ssh.sh"]
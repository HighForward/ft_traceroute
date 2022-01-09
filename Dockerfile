FROM debian:latest

RUN mkdir traceroute
COPY . traceroute/.

RUN chmod +x /traceroute/srcs/setup_ssh.sh

RUN apt-get update
RUN apt-get -y install openssh-server
RUN apt-get -y install traceroute
RUN apt-get -y install iputils-ping
RUN apt-get -y install gcc

EXPOSE 22

# CMD sleep 100000000
ENTRYPOINT ["/traceroute/srcs/setup_ssh.sh"]
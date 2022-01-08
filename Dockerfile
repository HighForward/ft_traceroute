FROM debian:latest

RUN mkdir traceroute
COPY srcs/. traceroute

RUN chmod +x /traceroute/setup_ssh.sh

RUN apt-get update
RUN apt-get -y install openssh-server
RUN apt-get -y install traceroute
RUN apt-get -y install iputils-ping
RUN apt-get -y install gcc

EXPOSE 22

ENTRYPOINT ["/traceroute/setup_ssh.sh"]
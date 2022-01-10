cmake_minimum_required(VERSION 3.10)
project(ft_traceroute C)

set(CMAKE_C_STANDARD 11)

add_executable(ft_traceroute
        srcs/main.c srcs/pkt.c srcs/utils.c srcs/dns.c srcs/args.c srcs/io_pkt.c)

#!/usr/bin/env python
from socket import *
from sys import *
from os import *
if len(argv)<3:
	raise "Require HOST and PORT"
sock=socket(AF_INET,SOCK_DGRAM);
sock.sendto(read(0,16384),(argv[1],int(argv[2])))

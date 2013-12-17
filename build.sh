#!/usr/bin/env bash
rm *~
for DIR in `ls`
do
	if [ -d $DIR ];
	then
		rm $DIR/*~
	fi
done
TARGET='run: bin/main\n\t./bin/main --verbose\n'
SOURCE=`ls src`
OBJECT=''

COMPILE='$(CC) $(CFLAGS) -c -o $@ $<\n'
LINK='$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIB)\n'

HEAD=''
BODY=''

HEAD+="CC=gcc\n"
HEAD+="CXX=g++\n"
HEAD+="CFLAGS=-I./inc -I/usr/include/libxml2 -O2 -pipe -march=native\n"
HEAD+="CXXFLAGS=${CXXFLAGS}\n"
HEAD+="LDFLAGS=\n"
HEAD+="LIB=-lGL -lglut -lGLEW -lxml2\n"

TARGET+='clean:\n\trm obj/*\nbin/main: '
for FILE in ${SOURCE}
do
	SRC="src/${FILE}"
	OBJ="obj/${FILE/.c/.o}"
	TARGET+="${OBJ} "
	BODY+="${OBJ}: ${SRC}\n\t${COMPILE}\n" 
done
TARGET+="\n\t${LINK}\n"
echo -e "${HEAD}\n${TARGET}\n${BODY}"

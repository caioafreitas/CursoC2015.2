SRC=src
INC=inc
OBJ=obj
DIST=dist
EXE=echo_client.exe
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
GCCARGS=-Wall -lws2_32

SRC=src
INC=inc
OBJ=obj
DIST=dist
EXE=echo_server
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
GCCARGS=-Wall

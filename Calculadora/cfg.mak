SRC=src
INC=inc
OBJ=obj
DIST=dist
EXE=calculadora.exe
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
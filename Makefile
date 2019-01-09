CC = gcc

DIR_INC = ./include
DIR_INC_BASE = ./celt
DIR_INC_LM = .//celt/fixed
DIR_SRC = ./src
DIR_ROOT = ./
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.c) \
	  $(wildcard ${DIR_ROOT}/celt/*.c) \
	  $(wildcard ${DIR_ROOT}/celt/fixed/*.c) \
	  $(wildcard ${DIR_ROOT}/silk/*.c) \
	  $(wildcard ${DIR_ROOT}/silk/fixed/*.c) \

OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

TARGET = opus_test

inclib = -lpthread

BIN_TARGET = ${DIR_BIN}/${TARGET}

ccflags-y += \
	-Icelt \
	-Icelt/fixed \
	-Isilk \
	-Isilk/fixed \
	-Iinclude \
	-Isrc 

#CFLAGS = -g -O2 -Wall -I${DIR_INC} -I${DIR_INC_BASE} -I${DIR_INC_LM}
ccflags-y += -g -O2 -Wall 

${BIN_TARGET} : ${OBJ}
	@echo 链接生成文件目标文件
	${CC} ${ccflags-y} ${OBJ} ${inclib} -o $@ -lm

${DIR_OBJ}/%.o : ${SRC} 
	@echo 编译文件开始..
	$(CC) ${ccflags-y} -c $?
	-mv *.o $(DIR_OBJ)/
	@echo 编译文件完成


#.PHONY : clean run
#	clean:
#		@echo 删除编译结果文件
#			rm -f ${DIR_OBJ}/*.o ${BIN_TARGET}

.PHONY:clean
clean:
	find ./ -name "*.o" -exec rm -rf "{}" \;
	#find ${DIR_OBJ} -name "*.o" -exec rm -rf "{}" \;

run:
	@echo 执行文件
	${BIN_TARGET}

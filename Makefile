CC = gcc

DIR_SRC = ./src
DIR_ROOT = ./
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_ROOT}/celt/*.c) \
	  $(wildcard ${DIR_ROOT}/celt/fixed/*.c) \
	  $(wildcard ${DIR_ROOT}/silk/*.c) \
	  $(wildcard ${DIR_ROOT}/silk/fixed/*.c) \
	  $(wildcard ${DIR_SRC}/*.c) \
	  $(wildcard ${DIR_ROOT}/test/*.c) \

OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

DEFS	= -DFIXED_POINT -DDISABLE_FLOAT_API -DOPUS_BUILD -DCUT=1

TARGET = opus

inclib = -lpthread

BIN_TARGET = ${DIR_BIN}/${TARGET}

ccflags-y += \
	-Iinclude \
	-Icelt \
	-Icelt/fixed \
	-Isilk \
	-Isilk/fixed \
	-Isrc 

ccflags-y += -g -Os -Wall

ccflags-y += $(DEFS) 

${BIN_TARGET} : ${OBJ}
	${CC} ${ccflags-y} ${OBJ} ${inclib} -o $@ -lm

${DIR_OBJ}/%.o : ${SRC} 
	$(CC) ${ccflags-y} -c $?
	-mv *.o $(DIR_OBJ)/


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

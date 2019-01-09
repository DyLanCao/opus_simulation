CC = gcc

DIR_INC = ./include
DIR_INC_BASE = ./include/sphinxbase
DIR_INC_LM = .//main/libsphinxbase/lm
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.c) \
	  	$(wildcard ${DIR_SRC}/libsphinxbase/util/*.c) \
	  	$(wildcard ${DIR_SRC}/libsphinxbase/feat/*.c) \
	  	$(wildcard ${DIR_SRC}/libsphinxbase/fe/*.c) \
	  	$(wildcard ${DIR_SRC}/libpocketsphinx/*.c) \
	  	$(wildcard ${DIR_SRC}/libsphinxbase/libsphinxad/*.c) \
	  	$(wildcard ${DIR_SRC}/libsphinxbase/lm/*.c)

OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

TARGET = continuous

inclib = -lpthread

BIN_TARGET = ${DIR_BIN}/${TARGET}

CFLAGS = -g -O2 -Wall -I${DIR_INC} -I${DIR_INC_BASE} -I${DIR_INC_LM}

${BIN_TARGET} : ${OBJ}
		@echo 链接生成文件目标文件
			${CC} ${CFLAGS} ${OBJ} ${inclib} -o $@ -lm

${DIR_OBJ}/%.o : ${SRC} 
		@echo 编译文件开始..
			$(CC) ${CFLAGS} -c $?
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

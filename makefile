# C compiler
CC = gcc

# Directory naming
BIN_DIR = bin
SRC_DIR = src

# Executables naming
SENDER_EXECUTABLE = sender
RECEIVER_EXECUTABLE = receiver

# Compiling extensions
COMPILATION_EXTENSIONS = -Wall -o

# The names of the .c source files
SENDER_SRC = ${SRC_DIR}/sender.c ${SRC_DIR}/sender/writenoncanonical.c ${SRC_DIR}/util/util.c
RECEIVER_SRC = ${SRC_DIR}/receiver.c ${SRC_DIR}/receiver/readnoncanonical.c ${SRC_DIR}/util/util.c
SRC_FILES = ${SENDER_SRC} ${RECEIVER_SRC}

make: ${SRC_FILES}
	@mkdir -p $(BIN_DIR)
	@$(CC) ${COMPILATION_EXTENSIONS} $(BIN_DIR)/${SENDER_EXECUTABLE} ${SENDER_SRC};
	@$(CC) ${COMPILATION_EXTENSIONS}  $(BIN_DIR)/${RECEIVER_EXECUTABLE} ${RECEIVER_SRC}

clean:
	@rm -f {BIN_DIR}/*
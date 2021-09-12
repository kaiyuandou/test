EXE_LOGGER = logger
EXE_NODE = node

CC = gcc
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter -Wmissing-declarations
CFLAGS_COMMON = $(WARNINGS) -std=c99 -D_GNU_SOURCE -pthread
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2
CFLAGS_DEBUG = $(CFLAGS_COMMON) -O0 -g -DDEBUG


release: bin $(EXE_LOGGER) $(EXE_NODE)
debug: bin clean $(EXE_LOGGER)-debug $(EXE_NODE)-debug

$(EXE_LOGGER): src/logger.c
	$(CC) $(CFLAGS_RELEASE) src/logger.c -o bin/$(EXE_LOGGER)

$(EXE_NODE): src/node.c
	$(CC) $(CFLAGS_RELEASE) src/node.c -o bin/$(EXE_NODE)

$(EXE_LOGGER)-debug: src/logger.c
	$(CC) $(CFLAGS_DEBUG) src/logger.c -o bin/$(EXE_LOGGER)

$(EXE_NODE)-debug: src/node.c
	$(CC) $(CFLAGS_DEBUG) src/node.c -o bin/$(EXE_NODE)

clean: 
	rm -rf bin/$(EXE_LOGGER) bin/$(EXE_NODE)

bin:
	mkdir -p bin

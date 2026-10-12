CC=clang
CFLAGS=-g -std=c11 -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wvla
LDFLAGS=-lm

LIB_DIR=lib
TESTS_DIR=tests
OBJECTS_DIR=objects
BIN_DIR=bin
VENDOR_DIR=vendor
OUT_DIR=out

LIB_SOURCES := $(shell find $(LIB_DIR) -name '*.c')
LIB_OBJECTS := $(patsubst $(LIB_DIR)/%.c,$(OBJECTS_DIR)/%.o,$(LIB_SOURCES))

TESTS_SOURCES := $(shell find $(TESTS_DIR) -name '*.c')
TESTS_OBJECTS := $(patsubst $(TESTS_DIR)/%.c,$(OBJECTS_DIR)/%.o,$(TESTS_SOURCES))
TESTS_BINS := $(patsubst $(TESTS_DIR)/%.c,$(BIN_DIR)/%,$(TESTS_SOURCES))
TESTS_TARGETS := $(addprefix run_target/,$(patsubst $(BIN_DIR)/%,%,$(filter %_test,$(TESTS_BINS))))

VENDOR_OBJECTS=$(wildcard $(VENDOR_DIR)/*.so) $(wildcard $(VENDOR_DIR)/*.a)

MAIN_SOURCE=main.c
MAIN_OBJECT=$(patsubst %.c,$(OBJECTS_DIR)/%.o,$(MAIN_SOURCE))
MAIN_BIN=$(patsubst $(OBJECTS_DIR)/%.o,$(BIN_DIR)/%,$(MAIN_OBJECT))
MAIN_TARGET=$(addprefix run_target_, $(notdir $(basename $(MAIN_BIN))))

.SECONDARY: $(TESTS_OBJECTS)

all: $(MAIN_BIN)

test: $(TESTS_BINS)

run: $(MAIN_TARGET)

runtest: $(TESTS_TARGETS)

static: $(OUT_DIR) $(LIB_OBJECTS)
	ar rcs $</$(notdir $(shell pwd)).a $(LIB_OBJECTS)

shared: $(OUT_DIR) $(LIB_OBJECTS)
	$(CC) $(CFLAGS) -shared -o $</$(notdir $(shell pwd)).so $(LIB_OBJECTS)

clean:
	rm -rf $(OBJECTS_DIR) $(BIN_DIR) $(OUT_DIR)

run_target_%: $(BIN_DIR)/%
	./$<

run_target/%: $(BIN_DIR)/%
	./$<

$(MAIN_OBJECT): $(MAIN_SOURCE) $(OBJECTS_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(MAIN_BIN): $(MAIN_OBJECT) $(LIB_OBJECTS) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LIB_OBJECTS) $(VENDOR_OBJECTS) $(LDFLAGS)

$(OBJECTS_DIR)/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

$(OBJECTS_DIR)/%.o: $(TESTS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/%: $(OBJECTS_DIR)/%.o $(LIB_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $< $(LIB_OBJECTS) $(LDFLAGS)

$(OBJECTS_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

$(OUT_DIR):
	mkdir -p $@

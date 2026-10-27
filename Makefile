CC = gcc
CXX = g++

CXXFLAGS = -std=c++23 \
           -Wall -Wextra \
           -fPIC \
           -Iinclude -Iinclude/marx/graphics-internal/freetype -Isrc -Idependencies

CFLAGS = -Wall -Wextra -fPIC -DFT2_BUILD_LIBRARY \
         -Iinclude -Iinclude/marx/graphics-internal/freetype -Isrc -Idependencies

LDFLAGS = -Wl,--as-needed
LDLIBS = -lX11 -lGL

PROJECT = $(notdir $(CURDIR))
LIBRARY = lib/lib$(PROJECT).so

TEMPLATE ?= window
TEMPLATE_SRC = templates/$(TEMPLATE).cpp
TEMPLATE_TARGET = bin/$(TEMPLATE)

FREETYPE_SRCS = $(addprefix ./src/freetype/, \
    base/ftsystem.c \
    base/ftinit.c \
    base/ftdebug.c \
    base/ftbase.c \
    base/ftbbox.c \
    base/ftbitmap.c \
    base/ftfstype.c \
    base/ftgasp.c \
    base/ftglyph.c \
    base/ftmm.c \
    gzip/ftgzip.c \
    lzw/ftlzw.c \
    psnames/psnames.c \
    psaux/psaux.c \
    pshinter/pshinter.c \
    cff/cff.c \
    sfnt/sfnt.c \
    smooth/smooth.c \
    truetype/truetype.c)

CPP_SRCS = $(shell find . -name '*.cpp' \
    -not -path './templates/*' \
    -not -path './build/*' \
    -not -path './bin/*' \
    -not -path './lib/*' \
    -not -path './.git/*' \
    -not -path './marx-graphics.backup/*')

C_SRCS = $(shell find . -name '*.c' \
    -not -path './templates/*' \
    -not -path './build/*' \
    -not -path './bin/*' \
    -not -path './lib/*' \
    -not -path './.git/*' \
    -not -path './marx-graphics.backup/*' \
    -not -path './src/freetype/*') \
    $(FREETYPE_SRCS)

OBJS = $(patsubst ./%.cpp,build/%.o,$(CPP_SRCS)) \
       $(patsubst ./%.c,build/%.o,$(C_SRCS))

.PHONY: all clean run

all: $(LIBRARY) $(TEMPLATE_TARGET)
	@printf '  Build complete\n'

# Build the shared library
$(LIBRARY): $(OBJS)
	@mkdir -p $(dir $@)
	@$(CXX) -shared $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)
	@printf '  Linked library %s\n' "$@"

# Build the selected template and link it against the shared library
$(TEMPLATE_TARGET): $(TEMPLATE_SRC) $(LIBRARY)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $< -Llib -l$(PROJECT) \
	    -Wl,-rpath,'$$ORIGIN/../lib' $(LDFLAGS) $(LDLIBS) -o $@
	@printf '  Linked template %s\n' "$@"

# Compile library C++ sources
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile library C sources
build/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

run: $(TEMPLATE_TARGET)
	@printf '  Running %s\n\n' "$(TEMPLATE_TARGET)"
	@./$(TEMPLATE_TARGET) $(ARGS); \
	Code=$$?; \
	printf '\n  Program exited with code %s\n' "$$Code"; \
	exit $$Code

clean:
	@rm -rf build bin lib
	@printf '  Cleaned build artifacts\n'

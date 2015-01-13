ifeq ($(OS),Windows_NT) # If windows
	LIB_ROOT = C:/dev/lib/x86
	INC_ROOT = C:/dev/include
	SDL_LFLAGS = -L$(LIB_ROOT) -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
	CC = gcc
else
	SDL_LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
	CC = clang
endif

MODE = DEBUG

ifeq ($(MODE),DEBUG)
    DEBUG_FLAGS = -g -DDEBUG
else
    DEBUG_FLAGS =
endif

OPT_LEVEL = 0
EXPLICIT_OPTIMIZATIONS =
STD = c99

TARGET		= runner
CFLAGS		= -std=$(STD) -O$(OPT_LEVEL) $(EXPLICIT_OPTIMIZATIONS) -Wall -I. -I$(INC_ROOT) $(DEBUG_FLAGS) -Wno-unknown-pragmas

LINKER		= $(CC) -o
LFLAGS		= -Wall -I. -lm

SRCDIR		= src
OBJDIR		= obj
BINDIR		= .

SOURCES		:= $(wildcard $(SRCDIR)/*.c)
INCLUDES	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS		:= $(SOURCES:$(SRCDIR)%.c=$(OBJDIR)/%.o)

all: $(BINDIR)/$(TARGET)
	@echo Done!

$(BINDIR)/$(TARGET): $(OBJECTS)
	@echo @$(LINKER) $@ $(LFLAGS) $(OBJECTS) $(SDL_LFLAGS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS) $(SDL_LFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo @$(CC) $(CFLAGS) -c $< -o $@
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONEY: clean
clean:
	@$(RM) $(OBJECTS)

.PHONEY: remove
remove: clean
	@$(RM) $(BINDIR)/$(TARGET)

.PHONEY: run
run: all
	./$(TARGET)

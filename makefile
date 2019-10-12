CC         = g++
CFLAGS     = -std=c++14 -Wall -Wextra -O3 -flto -DNDEBUG -DNULLMOVE -DASPIRATION_WINDOWS -DLMR -DFUTILITY_PRUNING

LINKER     = g++ -o
LFLAGS     = -L./libs/libataxx/build/static -pthread -lataxx

TARGET     = main
SRCDIR     = src
OBJDIR     = obj
BINDIR     = bin
INCDIR     = -I./libs/libataxx/src

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(BINDIR) $(OBJDIR) $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@
	@echo "Compiled "$<" successfully!"

bin:
	mkdir -p $(BINDIR)

obj:
	mkdir -p $(OBJDIR)

clean:
	rm -r $(OBJDIR)

.PHONY: clean

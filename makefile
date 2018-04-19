CC         = g++
CFLAGS     = -std=c++14 -Wall -Wextra -O3 -flto -DNDEBUG -DNULLMOVE -DASPIRATION_WINDOWS -DLMR

LINKER     = g++ -o
LFLAGS     = -pthread

TARGET     = main
SRCDIR     = src
OBJDIR     = obj
BINDIR     = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(BINDIR) $(OBJDIR) $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

bin:
	mkdir -p $(BINDIR)
obj:
	mkdir -p $(OBJDIR)

clean:
	rm -r $(OBJDIR)

.PHONY: clean

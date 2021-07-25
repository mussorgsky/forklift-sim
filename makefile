# project name (generate executable with this name)
TARGET   = forklift-sim

# compiling flags here
CXXFLAGS = -std=c++17 -O -pedantic -Wall -Wextra

LINKER   = g++
# linking flags here
LFLAGS   = -lsfml-system -lsfml-window -lsfml-graphics

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR) $(BINDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully"

.PHONY: clean
clean:
	@$(rm) -r $(OBJDIR)
	@echo "Cleanup complete"

.PHONY: remove
remove: clean
	@$(rm) -r $(BINDIR)
	@echo "Executable removed"


IDIR = includes
CC = g++
CXXFLAGS += -I$(IDIR) -std=c++11

ODIR = build
CPPDIR = src

_DEPS = main.h actions.h tools.h get.h setup.h player.h error.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o actions.o tools.o get.o setup.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(CPPDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CXXFLAGS)

game: $(OBJ)
	$(CC) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(INCDIR)/*~ rm game

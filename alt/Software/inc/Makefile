include Makefile.rules

all: $(LIB:%=lib/lib%.a)
	@echo "all libraries compiled."

lib/lib%.a: lib/%/*.cpp
	@for i in $^; do $(CC) $(CLIBOPTIONS) -I$(HEADER) -o $$i.o $$i; done
	ar r $@ $(^:.cpp=.cpp.o)
	@rm $(^:.cpp=.cpp.o)
	
clean: 
	rm lib/*.a

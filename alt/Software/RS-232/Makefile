INCPATH = ../inc/

include $(INCPATH)Makefile.rules

main: src/*.cpp Makefile $(INCPATH)header/*.h $(INCPATH)$(LIBRARIES)/*.a
	$(CC) src/*.cpp -I$(INCPATH)$(HEADER) -L$(INCPATH)$(LIBRARIES) -o main $(LIB:%=-l%) $(COPTIONS)
	
libraries:	
	make -C$(INCPATH) all
	
clean: 
	make -C$(INCPATH) clean

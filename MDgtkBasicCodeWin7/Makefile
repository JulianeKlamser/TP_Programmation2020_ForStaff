
CFLAGS= -Wall -ggdb -I. `pkg-config --cflags gtk+-3.0 gmodule-2.0`

XFLAGS =  -I. `pkg-config --cflags --libs  gtk+-3.0 gmodule-2.0`

HEADERS= file_picker_gui.h form_builder.h win_gui.h graphics.h
all : gtkcairo.exe 

OBJS = file_picker_gui.o form_builder.o win_gui.o graphics.o


gtkcairo.exe : gtkcairo.c $(OBJS) $(HEADERS)
	gcc  -o gtkcairo.exe $(OBJS) -Wall -g gtkcairo.c  -lm  $(XFLAGS)




clean:
	$(RM) -v *.o gtkcairo.exe


%.o : %.c ; gcc -c   $(CFLAGS) $< -o $@

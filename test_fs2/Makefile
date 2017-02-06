obj-m :=lwnfs.o

MY_CFLAGS+=-DDEBUG_OUTPUT 
ccflags-y+=${MY_CFLAGS}
CC+=${MY_CFLAGS}

ENABLE_DEBUG+=-g

all:
	#make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) EXTRA_CFLAGS="$(MY_CFLAGS)" modules

debug:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) EXTRA_CFLAGS="$(MY_CFLAGS) $(ENABLE_DEBUG)" modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

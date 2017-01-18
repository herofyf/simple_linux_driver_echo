obj-m+=idr.o
idr-objs:=test.o 

MY_CFLAGS+=-DDEBUG_OUTPUT
ccflags-y+=${MY_CFLAGS}
CC+=${MY_CFLAGS}


all:
	#make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) EXTRA_CFLAGS="$(MY_CFLAGS)" modules

debug:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) EXTRA_CFLAGS="$(MY_CFLAGS)" modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

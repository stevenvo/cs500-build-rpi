obj-y           += procfs_mod.o
KVERSION:=kernel
KDIR    := ${PRJROOT}/${KVERSION}
obj-m   := procfs_mod.o 

default:
        make ARCH=arm CROSS_COMPILE=${PREFIX}/bin/${TARGET}- -C $(KDIR) M=$(PWD\
) modules
clean:
        make -C $(KDIR) M=$(PWD) clean

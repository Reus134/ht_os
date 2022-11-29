#makefile得语法

##关于dd
这个的意思是产生一个 空的1440M的 file 从if就是 /dev/zero这是一个虚拟的空的文件夹 创建一个空的 到 of指向的目录
从这个里面 bs=512字节 count=2880 是个数 2880*512就是 1440M

==其实也就是创建一个软盘==
==bs代表blocksize==
是一个软盘镜像文件
dd if=/dev/zero of=$(BUILD_DIR)/main_floppy.img bs=512 count=2880
##mkfs.fat 创建一个文件系统
mkfs.fat -F 12 -n "NBOS" $(BUILD_DIR)/main_floppy.img

dd if=$(BUILD_DIR)/bootloader.bin of=$(BUILD_DIR)/main_floppy.img conv=notrunc
#上面这句的意思是 if是输入 of是输出 bootloader放大第一个 sector
linux dd命令中notrunc表示不要截断输出文件

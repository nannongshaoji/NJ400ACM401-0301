#
#  Makefile to build hello.c
#

# �������Ľ�����빤����
CROSS_COMPILE 	=mipsel-linux-
# windows�µĽ�����빤����
#CROSS_COMPILE 	=mips-linux-gnu-

#
# Include the make variables (CC, etc...)
#

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)mn
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
SIZE		= $(CROSS_COMPILE)size



# ���ļ�
LIB_HEADER_FILE = $(wildcard lib/*.h)
LIB_SRC = $(wildcard lib/*.S lib/*.c)

# ʾ���ļ�(������ʾ���ʹ�ÿ��ļ�)
EXAMPLE_HEADER_FILE = $(wildcard example/*.h)
EXAMPLE_SRC = $(wildcard example/*.c)

# Ӧ���ļ�
APP_HEADER_FILE = $(wildcard app/*.h)
APP_SRC = $(wildcard app/*.c )

# ���е��ļ�
ALL_HEADER_FILE = $(LIB_HEADER_FILE) $(EXAMPLE_HEADER_FILE) $(APP_HEADER_FILE)
ALL_SRC = $(LIB_SRC) $(EXAMPLE_SRC) $(APP_SRC)

# ���յ�Ŀ���ļ�
DEST = NJ400ACM-0301-V1.0

all:$(DEST)

$(DEST):$(ALL_SRC) $(ALL_HEADER_FILE)
#	${CC} -g -O2 -Wall -fno-pic -N -o $(DEST) $(ALL_SRC) -static -nostartfiles -e main -Wl,-m -Wl,elf32ltsmip -T ld.script
	${CC} -g -O2 -Wall -fno-pic -mno-abicalls  -N -o $(DEST) $(ALL_SRC) -static -nostartfiles -e main -Wl,-m -Wl,elf32ltsmip -T ld.script
#	cp $(DEST) /mnt/hgfs/ubuntuLTS16.04_x64/
clean:
	rm -f $(DEST) *.o
	
	

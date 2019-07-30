#CC =/home/f403tech/openwrt_widora/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-gcc
#AR = /home/f403tech/openwrt_widora/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-ar
CC = gcc
AR = ar
RM = rm -f	

TARGETLIB :=libsqlite3.so

INC_DIR += 

SRC += sqlite3.c

OBJS =  $(SRC:.c=.o)

%.o: %.c
	$(CC) -c -fPIC $(INC_DIR) $< -o $@ 

all: $(TARGETLIB)
$(TARGETLIB):	$(OBJS)
	$(CC) -shared -o $(TARGETLIB) $?
	$(RM) $(OBJS)
	
clean:
	$(RM) $(TARGETLIB)
	$(RM) $(OBJS)

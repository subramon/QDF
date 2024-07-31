# Why are we depending on print and load csv?
CFLAGS := -g -O0
# CFLAGS := -O3
CFLAGS += -DDEBUG

CFLAGS += -fopenmp
CFLAGS += -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align  
CFLAGS += -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings 
CFLAGS += -Wunused-variable -Wunused-parameter -Wno-pedantic 
CFLAGS += -Wno-unused-label 
# CFLAGS += -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined 
CFLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith 
CFLAGS += -Wmissing-declarations -Wredundant-decls -Wnested-externs 
CFLAGS += -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings 
CFLAGS += -Wold-style-definition 
# not for mac CFLAGS += -Wsuggest-attribute=noreturn -Wduplicated-cond 
# not for mac CFLAGS += -Wmisleading-indentation 
# not for mac CFLAGS += -Wduplicated-branches -Wrestrict
# not for CentOS CFLAGS += -Wnull-dereference 
#
INCS := -I../inc/ 

all: libutils.so utils_hdrs.lua  \
	charsub cli_mk_file cli_rmtree ut_fp32tobf16  \
	cli_strip_upto 

CC=gcc
.c.o :
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

conv_for_R.o : CFLAGS += -Wno-cast-align
tm2time.o : CFLAGS += -Wno-cast-qual
spooky_hash.o : CFLAGS += -Wno-implicit-fallthrough -Wno-cast-qual

utils_hdrs.lua : ../inc/num_lines.h \
	../inc/num_cols.h 
	echo "return [["       >  utils_hdrs.lua
	cat ../inc/num_lines.h >> utils_hdrs.lua
	cat ../inc/file_exists.h >> utils_hdrs.lua
	cat ../inc/num_cols.h  >> utils_hdrs.lua
	cat ../inc/read_csv.h  >> utils_hdrs.lua
	cat ../inc/split_str.h  >> utils_hdrs.lua
	echo "]]"              >> utils_hdrs.lua 

unique.o: CFLAGS += -Wno-cast-align

lookup8.o: CFLAGS += -Wno-implicit-fallthrough -Wno-cast-align

clean:
	rm -f *.so *.o utils_hdrs.lua charsub cli_mk_file cli_rmtree

UTILS_SRC := auxil.c 
UTILS_SRC += bf16tofp32.c 
UTILS_SRC += conv_for_R.c 
UTILS_SRC += copy_array.c 
UTILS_SRC += date_str_to_epoch.c 
UTILS_SRC += file_as_str.c 
UTILS_SRC += file_exists.c 
UTILS_SRC += fp32tobf16.c
UTILS_SRC += get_file_size.c
UTILS_SRC += get_ival.c 
UTILS_SRC += get_time_usec.c 
UTILS_SRC += lookup8.c 
UTILS_SRC += isdir.c 
UTILS_SRC += line_to_cells.c 
UTILS_SRC += load_lines.c 
UTILS_SRC += mk_file.c 
UTILS_SRC += mk_temp_file.c 
UTILS_SRC += num_cols.c 
UTILS_SRC += num_lines.c 
UTILS_SRC += qtypes.c
UTILS_SRC += read_csv.c
UTILS_SRC += rmtree.c
UTILS_SRC += rs_mmap.c
UTILS_SRC += sort1.c
UTILS_SRC += split_str.c
UTILS_SRC += str_between.c
UTILS_SRC += str_as_file.c
UTILS_SRC += tm2time.c
UTILS_SRC += unique.c
UTILS_OBJ = $(UTILS_SRC:.c=.o)

libutils.so: $(UTILS_OBJ) 
	gcc ${CFLAGS} $(INCS) $(UTILS_OBJ) -shared -o libutils.so -lm

charsub: rs_mmap.o charsub.o
	gcc ${CFLAGS} $(INCS) rs_mmap.o charsub.o -o charsub

cli_mk_file: cli_mk_file.o libutils.so 
	gcc ${CFLAGS} $(INCS) cli_mk_file.o -o cli_mk_file libutils.so

cli_rmtree: cli_rmtree.o libutils.so 
	gcc ${CFLAGS} $(INCS) cli_rmtree.o -o cli_rmtree libutils.so

cli_strip_upto: cli_strip_upto.o 
	gcc ${CFLAGS} $(INCS) cli_strip_upto.o -o cli_strip_upto 

ut_fp32tobf16: test_fp32tobf16.o libutils.so 
	gcc ${CFLAGS} $(INCS) test_fp32tobf16.o -o ut_fp32tobf16 libutils.so


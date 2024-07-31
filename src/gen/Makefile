CC = gcc
CFLAGS := -g 
CFLAGS := -g -O0 # optimizations cause buffer overflow on maas
# CFLAGS := -O4

CFLAGS += -DDEBUG

CFLAGS += -fopenmp
CFLAGS += -DISPC
CFLAGS += -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align 
CFLAGS += -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings 
CFLAGS += -Wunused-variable -Wunused-parameter 
CFLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith 
CFLAGS += -Wmissing-declarations -Wredundant-decls -Wnested-externs 
CFLAGS += -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings 
CFLAGS += -Wold-style-definition 
CFLAGS += -Wattributes
CFLAGS += -Wunused-label 
CFLAGS += -Wconversion
CFLAGS += -Wnull-dereference # Comment for CentOS 
CFLAGS += -Wpedantic 
CFLAGS += -Wuninitialized
#---------------
CFLAGS += -Wno-incompatible-pointer-types # Comment for CentOS
CFLAGS += -Wno-cast-align
CFLAGS += -Wno-aggregate-return
# CFLAGS += -Wno-discarded-qualifiers
CFLAGS += -Wno-ignored-qualifiers


all : libgen.so

clean :
	rm -f _gen* libgen.so  libispc.so
	
full_clean :
	rm -f _gen* libgen.so 
	rm -f ./src/*.c 
	rm -f ./src/*.o
	rm -f ./inc/*.h
	rm -f ./ispc/*.ispc
	rm -f ./ispc/*.h

#
INCS := -I${RSUTILS_SRC_ROOT}/inc/   
INCS += -I${SCLR_SRC_ROOT}/inc/   
INCS += -I../../inc/  
INCS += -I./inc/  # for generated files
INCS += -I./ispc/  # for generated files

.c.o :
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

#----------------------------------------------------------
./src/gen2.c : gen2.lua tmpl2.c.lua 
	luajit gen2.lua 
GEN2_SRC := ./src/gen2.c
GEN2_OBJ = $(GEN2_SRC:.c=.o)
#----------------------------------------------------------
./src/gen3.c : gen3.lua tmpl3.c.lua 
	luajit gen3.lua 
GEN3_SRC := ./src/gen3.c
GEN3_OBJ = $(GEN3_SRC:.c=.o)
#----------------------------------------------------------
./src/gen4.c : gen4.lua tmpl4_seq.c.lua tmpl4_incr_on_by.c.lua
	luajit gen4.lua 
GEN4_SRC := ./src/gen4.c
GEN4_OBJ = $(GEN4_SRC:.c=.o)
#----------------------------------------------------------
./src/gen5.c : gen5.lua tmpl5.c.lua 
	luajit gen5.lua 
GEN5_SRC := ./src/gen5.c
GEN5_OBJ = $(GEN5_SRC:.c=.o)
#----------------------------------------------------------
./src/gen6.c : gen6.lua tmpl6.c.lua 
	luajit gen6.lua 
GEN6_SRC := ./src/gen6.c
GEN6_OBJ = $(GEN6_SRC:.c=.o)
#----------------------------------------------------------
./src/gen6a.c : gen6a.lua tmpl6a.c.lua 
	luajit gen6a.lua 
GEN6A_SRC := ./src/gen6a.c
GEN6A_OBJ = $(GEN6A_SRC:.c=.o)
#----------------------------------------------------------
./src/gen6b.c : gen6b.lua tmpl6b.c.lua 
	luajit gen6b.lua 
GEN6B_SRC := ./src/gen6b.c
GEN6B_OBJ = $(GEN6B_SRC:.c=.o)
#----------------------------------------------------------
./src/gen6c.c : gen6c.lua tmpl6c.c.lua 
	luajit gen6c.lua 
GEN6C_SRC := ./src/gen6c.c
GEN6C_OBJ = $(GEN6C_SRC:.c=.o)
#----------------------------------------------------------
./src/gen8.c : gen8.lua tmpl8.c.lua 
	luajit gen8.lua 
GEN8_SRC := ./src/gen8.c
GEN8_OBJ = $(GEN8_SRC:.c=.o)
#----------------------------------------------------------
./src/gen7.c : gen7.lua tmpl7.c.lua 
	luajit gen7.lua 
GEN7_SRC := ./src/gen7.c
GEN7_OBJ = $(GEN7_SRC:.c=.o)
#----------------------------------------------------------
./src/gen7a.c : gen7a.lua tmpl7a.c.lua 
	luajit gen7a.lua 
GEN7A_SRC := ./src/gen7a.c
GEN7A_OBJ = $(GEN7A_SRC:.c=.o)
#----------------------------------------------------------
./src/gen7b.c : gen7b.lua tmpl7b.c.lua 
	luajit gen7b.lua 
GEN7B_SRC := ./src/gen7b.c
GEN7B_OBJ = $(GEN7B_SRC:.c=.o)
#----------------------------------------------------------
./src/gen1a.c : gen1a.lua tmpl1a.c.lua 
	luajit gen1a.lua 
GEN1A_SRC := ./src/gen1a.c
GEN1A_OBJ = $(GEN1A_SRC:.c=.o)
#----------------------------------------------------------
./src/gen1d.c : gen1d.lua tmpl1d.c.lua 
	luajit gen1d.lua 
GEN1D_SRC := ./src/gen1d.c
GEN1D_OBJ = $(GEN1D_SRC:.c=.o)
#----------------------------------------------------------
./src/gen1.c : gen1.lua tmpl1.c.lua 
	luajit gen1.lua 
GEN1_SRC := ./src/gen1.c
GEN1_OBJ = $(GEN1_SRC:.c=.o)
#----------------------------------------------------------

libgen.so: libispc.so \
	${GEN1_SRC} \
	${GEN1A_SRC} \
	${GEN1D_SRC} \
	${GEN2_SRC} \
	${GEN3_SRC} \
	${GEN4_SRC} \
	${GEN5_SRC} \
	${GEN6_SRC} \
	${GEN6A_SRC} \
	${GEN6B_SRC} \
	${GEN6C_SRC} \
	${GEN7_SRC} \
	${GEN7A_SRC} \
	${GEN7B_SRC} \
	${GEN8_SRC} 
	gcc -DISPC ${CFLAGS} ${INCS} -fPIC -shared -o libgen.so \
	${GEN1_SRC} \
	${GEN1A_SRC} \
	${GEN1D_SRC} \
	${GEN2_SRC} \
	${GEN3_SRC} \
	${GEN4_SRC} \
	${GEN5_SRC} \
	${GEN6_SRC} \
	${GEN6A_SRC} \
	${GEN6B_SRC} \
	${GEN6C_SRC} \
	${GEN7_SRC} \
	${GEN7A_SRC} \
	${GEN7B_SRC} \
	${GEN8_SRC} \
	libispc.so  -lm

#----------------------------------
ISPC1_SRC = ispc/gen1d.ispc
ISPC1_OBJ = $(ISPC1_SRC:.ispc=.o)

${ISPC1_SRC} : 
	luajit gen1d_ispc.lua
${ISPC1_OBJ} : ${ISPC1_SRC}
	ispc ${ISPC1_SRC} -o ${ISPC1_OBJ}
#----------------------------------
ISPC7_SRC = ispc/gen7.ispc
ISPC7_OBJ = $(ISPC7_SRC:.ispc=.o)

${ISPC7_SRC} : 
	luajit gen7_ispc.lua
${ISPC7_OBJ} : ${ISPC7_SRC}
	ispc ${ISPC7_SRC} -o ${ISPC7_OBJ}
#----------------------------------


ALL_ISPC_OBJ := ${ISPC1_OBJ} ${ISPC7_OBJ}
# here is where you add other ispc stuff 

libispc.so : ${ALL_ISPC_OBJ}
	gcc  -fPIC -shared ${ALL_ISPC_OBJ} -o libispc.so 

# libgen.so :
# 	gcc -o libgen.so  -fPIC -shared src/gen*.c ${INCS} ${CFLAGS}

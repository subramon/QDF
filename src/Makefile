# CFLAGS := -g 
CFLAGS := -g -O0 # optimizations cause buffer overflow on maas
# CFLAGS := -O3

CFLAGS += -DDEBUG
CFLAGS += -DISPC
CFLAGS += -fopenmp
CFLAGS += -fstrict-aliasing 
CFLAGS += -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align 
CFLAGS += -Wstrict-aliasing
# CFLAGS += -Wincompatible-pointer-types # comment for CentOS
CFLAGS += -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings 
CFLAGS += -Wunused-variable -Wunused-parameter 
CFLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith 
CFLAGS += -Wmissing-declarations -Wredundant-decls -Wnested-externs 
CFLAGS += -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings 
CFLAGS += -Wold-style-definition 
CFLAGS += -Wattributes
CFLAGS += -Wunused-label 
# CFLAGS += -Wint-conversion # comment for CentOS
CFLAGS += -Wconversion
# CFLAGS += -Wnull-dereference  # comment for CentOS
CFLAGS += -Wpedantic 
CFLAGS += -Wuninitialized
#---------------
# WHY DID WE UNDO THIS CHECK? CFLAGS += -Wno-incompatible-pointer-types
# WHY DID WE UNDO THIS CHECK? CFLAGS += -Wno-cast-align
# WHY DID WE UNDO THIS CHECK? CFLAGS += -Wno-aggregate-return
# CFLAGS += -Wno-discarded-qualifiers
# WHY DID WE UNDO THIS CHECK? CFLAGS += -Wno-ignored-qualifiers
# not for mac CFLAGS += -Wsuggest-attribute=noreturn -Wduplicated-cond 
# not for mac CFLAGS += -Wmisleading-indentation 
# not for mac CFLAGS += -Wduplicated-branches -Wrestrict
#

INCS := -I. # for globals.h 
INCS += -I../inc/ 
INCS += -I./gen/inc/  # for generated files
INCS += -I./gen/ispc/  # for generated files
INCS += -I${CMEM_SRC_ROOT}/inc/ 
INCS += -I${SCLR_SRC_ROOT}/inc/ 
INCS += -I${C2R_SRC_ROOT}/inc/ 
INCS += -I${C2R_SRC_ROOT}/rincs/ 
INCS += -I${RSUTILS_SRC_ROOT}/inc/ 
INCS += -I/usr/include/lua5.1/ # for lua.h

GEN_LIBS = libgen.so libispc.so # when ispc is installed
GEN_LIBS = libgen.so            # when ispc is NOT installed
LCL_LIBS = ${GEN_LIBS} libqdf.so 
LUAJIT_SO := /usr/lib/x86_64-linux-gnu/libluajit-5.1.so.2
#LUAJIT_SO := /usr/local/lib/libluajit-5.1.so
LIBS := -lm 
LIBS += -lpthread 
LIBS += -ljansson
LIBS += -levent 
LIBS += ${LUAJIT_SO}

# TODO ALL UNIT TESTS 
UNIT_TESTS := ut_concat 
UNIT_TESTS += ut_copy 
UNIT_TESTS += ut_jni 
UNIT_TESTS += ut_line_to_cells
UNIT_TESTS += ut_logical_op 
UNIT_TESTS += ut_qdf_makers 
UNIT_TESTS += ut_mixed 
UNIT_TESTS += ut_pr 
UNIT_TESTS += ut_qdf_named_vec
UNIT_TESTS += ut_read_csv 
UNIT_TESTS += ut_squeeze 
UNIT_TESTS += ut_tm 
UNIT_TESTS += ut_unique 
#--------
UNIT_TESTS += ut_raw_to_csv

LUA_HDRS := ../lua/qdf_hdrs.lua 
UTIL_HDRS := ../lua/util_hdrs.lua 

CLI += cli_get_dataset 
CLI += cli_get_stats 
CLI += cli_get_info 
CLI += cli_pr_df 
CLI += cli_csv_to_qdf 
CLI += cli_check 

all: libqdf.so  ${GEN_LIBS} ${LUA_HDRS} ${UTIL_HDRS} # TODO  ${UNIT_TESTS} 

CC=gcc
# Use clang as an extra compile time check
# CC=clang
# CFLAGS=-Weverything -Wno-switch-enum


.c.o :
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

_stripped_cmem.h : ${CMEM_SRC_ROOT}/inc/cmem_struct.h 
	luajit cli_for_cdef.lua ${CMEM_SRC_ROOT}/inc/cmem_struct.h _stripped_cmem.h

_stripped_sclr.h : ${SCLR_SRC_ROOT}/inc/sclr_struct.h 
	luajit cli_for_cdef.lua ${SCLR_SRC_ROOT}/inc/sclr_struct.h _stripped_sclr.h

../lua/util_hdrs.lua :  ${RSUTILS_SRC_ROOT}/inc/read_csv.h 
	cat ${RSUTILS_SRC_ROOT}/inc/read_csv.h > ../lua/util_hdrs.lua 

../lua/qdf_hdrs.lua : \
		${RSUTILS_SRC_ROOT}/inc/qtypes.h \
		_stripped_cmem.h \
		_stripped_sclr.h \
		../inc/qdf_struct.h \
		../inc/qdf_makers.h \
		../inc/qdf_checkers.h \
		../inc/qdf_const.h \
		../inc/qdf_seq.h \
		../inc/qdf_incr_on_by.h \
		../inc/qdf_is_sawtooth.h \
		../inc/qdf_grp_sawtooth.h \
		../inc/qdf_grp_flatten_sawtooth.h \
		../inc/qdf_vals_counts.h \
		../inc/qdf_vals_sums.h \
		../inc/qdf_copy.h \
		../inc/qdf_clone.h \
		../inc/qdf_external.h \
		../inc/qdf_get_by_idx.h \
		../inc/qdf_helpers.h \
		../inc/qdf_xhelpers.h \
		../inc/qdf_join.h \
		../inc/qdf_mem.h \
		../inc/qdf_vs_op.h \
		../inc/qdf_f_to_s.h \
		../inc/qdf_vv_op.h \
		../inc/qdf_v_op.h \
		./gen/inc/gen8_lua.h \
		../inc/qdf_ifxthenyelsez.h \
		../inc/qdf_compress.h \
		../inc/qdf_decompress.h \
		../inc/qdf_permute.h \
		../inc/qdf_fill.h \
		../inc/qdf_srt_grp_cnt.h \
		../inc/qdf_coalesce.h \
		../inc/qdf_nop.h \
		../inc/qdf_pr.h \
		../inc/qdf_prefix_sums.h \
		../inc/qdf_setters.h \
		../inc/qdf_shift_up_down.h \
		../inc/qdf_next_prev.h \
		../inc/qdf_smear.h \
		../inc/qdf_sort.h \
		../inc/qdf_squeeze_where.h \
		../inc/qdf_tm.h \
		../inc/qdf_write.h \
		../inc/qdf_resize_df.h \
		../inc/qdf_append.h \
		../inc/qdf_uq.h 
	echo "return [["       > ../lua/qdf_hdrs.lua
	grep -v "^#" ${RSUTILS_SRC_ROOT}/inc/qtypes.h >> ../lua/qdf_hdrs.lua
	cat _stripped_cmem.h >> ../lua/qdf_hdrs.lua
	cat _stripped_sclr.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_struct.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_makers.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_helpers.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_xhelpers.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_const.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_seq.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_incr_on_by.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_is_sawtooth.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_grp_sawtooth.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_grp_flatten_sawtooth.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_vals_counts.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_vals_sums.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_checkers.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_get_by_idx.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_compress.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_decompress.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ./gen/inc/gen8_lua.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_permute.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_fill.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_srt_grp_cnt.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_mem.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_vs_op.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_f_to_s.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_vv_op.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_v_op.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_ifxthenyelsez.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_coalesce.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_setters.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_squeeze_where.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_pr.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_prefix_sums.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_resize_df.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_append.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_copy.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_clone.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_join.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_smear.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_sort.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_external.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_shift_up_down.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_next_prev.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_tm.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_write.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_uq.h >> ../lua/qdf_hdrs.lua
	grep -v "^#" ../inc/qdf_nop.h >> ../lua/qdf_hdrs.lua
	echo "]]"             >> ../lua/qdf_hdrs.lua

clean:
	rm -f *.o *.so _* ${UNIT_TESTS} 
	rm -f ${LUA_HDRS} 

qdf_SRC += qdf_makers.c 
qdf_SRC += qdf_append.c 
qdf_SRC += qdf_resize_df.c 
qdf_SRC += qdf_checkers.c 
qdf_SRC += qdf_concat.c 
qdf_SRC += qdf_const.c 
qdf_SRC += qdf_seq.c 
qdf_SRC += qdf_incr_on_by.c 
qdf_SRC += qdf_is_sawtooth.c 
qdf_SRC += qdf_grp_sawtooth.c 
qdf_SRC += qdf_grp_flatten_sawtooth.c 
qdf_SRC += qdf_vals_counts.c 
qdf_SRC += qdf_vals_sums.c 
qdf_SRC += qdf_copy.c 
qdf_SRC += qdf_clone.c 
qdf_SRC += qdf_external.c 
qdf_SRC += qdf_get_by_idx.c 
qdf_SRC += qdf_helpers.c 
qdf_SRC += qdf_xhelpers.c 
qdf_SRC += qdf_join.c 
qdf_SRC += qdf_vs_op.c 
qdf_SRC += vsraised_to.c 
qdf_SRC += sawtooth.c 
qdf_SRC += flatten_sawtooth.c 
qdf_SRC += qdf_f_to_s.c 
qdf_SRC += qdf_vv_op.c 
qdf_SRC += qdf_v_op.c 
qdf_SRC += qdf_ifxthenyelsez.c 
qdf_SRC += qdf_compress.c 
qdf_SRC += qdf_decompress.c 
qdf_SRC += qdf_permute.c 
qdf_SRC += qdf_fill.c 
qdf_SRC += qdf_mem.c 
qdf_SRC += qdf_srt_grp_cnt.c 
qdf_SRC += qdf_coalesce.c 
qdf_SRC += qdf_named_vec.c 
qdf_SRC += qdf_nop.c 
qdf_SRC += qdf_pr.c 
qdf_SRC += qdf_csv_to_df.c 
qdf_SRC += qdf_prefix_sums.c 
qdf_SRC += qdf_setters.c 
qdf_SRC += qdf_shift_up_down.c 
qdf_SRC += qdf_next_prev.c 
qdf_SRC += qdf_smear.c 
qdf_SRC += qdf_sort.c 
qdf_SRC += qdf_squeeze_where.c 
qdf_SRC += qdf_tm.c 
qdf_SRC += qdf_uq.c 
qdf_SRC += qdf_write.c 
# qdf_SRC   += cli/wrap_pr_df.c 
# qdf_SRC   += cli/wrap_csv_to_qdf.c 
qdf_OBJ  = $(qdf_SRC:.c=.o)

#------------------------------------
C2R_SO=${C2R_SRC_ROOT}/libc2r.so
${C2R_SO} :
	make -C ${C2R_SRC_ROOT}
#------------------------------------
SCLR_SO=${SCLR_SRC_ROOT}/src/libsclr.so
${SCLR_SO} :
	make -C ${SCLR_SRC_ROOT}/src/
#------------------------------------
CMEM_SO=${CMEM_SRC_ROOT}/src/libcmem.so
${CMEM_SO} :
	make -C ${CMEM_SRC_ROOT}/src/
#------------------------------------
RSUTILS_SO=${RSUTILS_SRC_ROOT}/src/librsutils.so
${RSUTILS_SO} :
	make -C ${RSUTILS_SRC_ROOT}/src/


libqdf.so: ${qdf_OBJ} ${GEN_LIBS} ${C2R_SO} ${CMEM_SO} ${SCLR_SO} ${RSUTILS_SO}
	gcc ${CFLAGS} $(INCS) ${qdf_OBJ} ${GEN_LIBS} -shared -o libqdf.so \
	-lm ${C2R_SO} ${CMEM_SO} ${SCLR_SO} ${RSUTILS_SO}

libispc.so : 
	make -C ./gen/ libispc.so 
	cp ./gen/libispc.so  .

libgen.so: 
	make -C ./gen/
	cp ./gen/libgen.so  .

cli_pr_df: ./cli/cli_pr_df.c libqdf.so $(LUA_HDRS)
	gcc ${CFLAGS} $(INCS) \
		./cli/cli_pr_df.c \
		libqdf.so \
		-o cli_pr_df 

cli_csv_to_qdf: ./cli/cli_csv_to_qdf.c libqdf.so $(LUA_HDRS)
	gcc ${CFLAGS} $(INCS) \
		./cli/cli_csv_to_qdf.c \
		libqdf.so \
		-o cli_csv_to_qdf 

qdf_makers.o : CFLAGS += -Wno-conversion
qdf_checkers.o : CFLAGS += -Wno-conversion
qdf_concat.o : CFLAGS += -Wno-conversion
qdf_helpers.o : CFLAGS += -Wno-conversion

ut_merge_df: ../test/ut_merge_df.c libqdf.so 
	gcc ${CFLAGS} $(INCS) \
		../test/ut_merge_df.c \
		libqdf.so librcli.so \
		/usr/local/lib/libluajit-5.1.so \
		-o ut_merge_df 

ut_qdf_makers: libqdf.so ../test/ut_qdf_makers.c 
	gcc ${CFLAGS} $(INCS)  ../test/ut_qdf_makers.c  \
		-o ut_qdf_makers libqdf.so ${LIBS}

SRCS_UT_READ_CSV += qdf_helpers.c 
SRCS_UT_READ_CSV += qdf_checkers.c 
SRCS_UT_READ_CSV += qdf_external.c 
SRCS_UT_READ_CSV += qdf_setters.c 
SRCS_UT_READ_CSV += qdf_pr.c 
SRCS_UT_READ_CSV += qdf_makers.c 
SRCS_UT_READ_CSV += logical_op.c 
SRCS_UT_READ_CSV += sum.c 
SRCS_UT_READ_CSV += squeeze_where.c 

ut_read_csv: ../test/ut_read_csv.c  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_read_csv.c   \
		-o ut_read_csv libqdf.so ${LIBS}

ut_logical_op: ../test/ut_logical_op.o  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_logical_op.o   \
		-o ut_logical_op libqdf.so ${LIBS}

ut_qdf_named_vec: ../test/ut_qdf_named_vec.c  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_qdf_named_vec.c   \
		-o ut_qdf_named_vec libqdf.so ${LIBS}

ut_unique:  ../test/ut_unique.c  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_unique.c   \
		-o ut_unique libqdf.so  ${LIBS}

ut_squeeze:  ../test/ut_squeeze.c  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_squeeze.c   \
		-o ut_squeeze libqdf.so  ${LIBS}

ut_tm:  ../test/ut_tm.c  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_tm.c   \
		-o ut_tm libqdf.so  ${LIBS}

ut_copy:  ../test/ut_copy.c  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_copy.c   \
		-o ut_copy libqdf.so  ${LIBS}

ut_concat:  ../test/ut_concat.c  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_concat.c   \
		-o ut_concat \
		libqdf.so  ${GEN_LIBS} ${LIBS}

ut_pr:  ../test/ut_pr.c  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_pr.c   \
		-o ut_pr libqdf.so  ${LIBS}

ut_mixed:  ../test/ut_mixed_array.c  libqdf.so 
	gcc ${CFLAGS} $(INCS) ../test/ut_mixed_array.c   \
		-o ut_mixed libqdf.so  ${LIBS}

ut_line_to_cells:  ../test/ut_line_to_cells.c 
	gcc ${CFLAGS} $(INCS) ../test/ut_line_to_cells.c   \
	  	dsid.o \
		-o ut_line_to_cells  ${LIBS}



cli_check:  libqdf.so libmtmgcv.so \
	./cli/cli_check.o
	gcc ${CFLAGS} $(INCS) \
		./cli/cli_check.c  \
		 libqdf.so libmtmgcv.so \
		-o cli_check ${LIBS}

# Explanation: the linking is dependent on the order of modules.  # Symbols are first requested, and then linked in from a library 
# that has them. So you have to specify modules that use libraries first, 
# and libraries after them. Like this:
# 
# gcc x.o y.o z.o -la -lb -lc
# 
# Moreover, in case there's a circular dependency, you should specify 
# the same library on the command line several times. 
# So in case libb needs symbol from libc and libc needs symbol from libb, 
# the command line should be:
# 
# gcc x.o y.o z.o -la -lb -lc -lb
# 

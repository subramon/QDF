#!/bin/bash
set -e
# WORKS luajit test_qdf.lua
# WORKS luajit test_append.lua
# WORKS luajit test_clamp.lua
# WORKS luajit test_qdf_csv.lua
# WORKS luajit test_clone.lua
# WORKS luajit test_coalesce.lua
# WORKS luajit test_const.lua
# WORKS luajit test_convert.lua
# WORKS luajit test_fill.lua
# WORKS luajit test_ifxthenyelsez.lua
# WORKS luajit test_join.lua
luajit test_logical_op.lua
echo "PREMATURE"; exit 1
luajit test_log.lua
luajit test_make_empty_data_frame.lua
luajit test_min_max.lua
luajit test_mk_sort_idx_2.lua # TODO PARTIAL 
luajit test_mul_div.lua
luajit test_num_unique.lua
luajit test_pr_csv.lua
luajit test_pr_df_as_csv.lua
# NOTE: luajit test_qdfmem.lua #- enable cdefs before this will work 
luajit test_read_configs.lua
luajit test_read_csv.lua
luajit test_round.lua
luajit test_set.lua
luajit test_set_nn.lua
luajit test_shift.lua #- test shift up down 
luajit test_shift_mask.lua
luajit test_simdjson.lua
# SHOULD WE DEPRECATE??? DO WE NEED FILL? luajit test_smear.lua
luajit test_sort.lua
luajit test_squeeze.lua
luajit test_time_band.lua
luajit test_vals_counts.lua
luajit test_vals_sums.lua
luajit test_week_to_hol.lua
luajit test_write.lua
luajit test_logical_op.lua
echo "====================================================="
luajit test_read_csv.lua
# TODO NEED TO CREATE A NEW 0.qdf file luajit test_binld.lua
# TODO luajit test_compress.lua
# TODO luajit test_df_compress.lua
# TODO luajit test_prefix_sums.lua
# TODO WIP seg fault luajit test_place_in_data_frame.lua
echo "Completed $0 in $PWD successfully"

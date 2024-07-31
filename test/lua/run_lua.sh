#!/bin/bash
set -e
luajit test_append.lua
# TODO NEED TO CREATE A NEW 0.rbc file luajit test_binld.lua
luajit test_calc_offset.lua
luajit test_chk1.lua
luajit test_clamp.lua
luajit test_clone.lua
luajit test_coalesce.lua
# TODO luajit test_compress.lua
luajit test_const.lua
luajit test_convert.lua
luajit test_fill.lua
# TODO luajit test_df_compress.lua
luajit test_holiday_extract.lua
luajit test_ifxthenyelsez.lua
luajit test_join.lua
luajit test_logical_op.lua
luajit test_log.lua
luajit test_make_empty_data_frame.lua
luajit test_min_max.lua
luajit test_mk_sort_idx_2.lua # TODO PARTIAL 
luajit test_mul_div.lua
luajit test_num_unique.lua
# WIP seg fault luajit test_place_in_data_frame.lua
luajit test_pr_csv.lua
luajit test_pr_df_as_csv.lua
# SHOULD WE DEPRECATE? DO WE NEED FILL? luajit test_prefix_sums.lua
luajit test_RBC_csv.lua
luajit test_RBC.lua
# NOTE: luajit test_rbcmem.lua #- enable cdefs before this will work 
luajit test_rbc_mgcv_model.lua
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
luajit test_RBC.lua
echo "Completed $0 in $PWD successfully"

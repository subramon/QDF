frmla = ' sls_unit_q ~ s(co_loc_i, bs = "re") + offset(promo_price_lift) +
  columbusday + goodfriday + easter + fathersday_minus + fathersday +
  laborday_minus + laborday + mardigras + memorialday_minus +
  memorialday + mlkday + mothersday_minus + mothersday + presidentsday +
  superbowl_minus + superbowl + thanksgiving_minus + thanksgiving +
  cyber_monday + valentines_minus + valentines + stpatricks +
  cincodemayo + julyfourth_minus + julyfourth + halloween_minus +
  halloween + veteransday + christmas_minus + christmas + newyearsday_minus +
  newyearsday + s(t_o_y, bs = "cc", k = 25) +
  s(sls_unit_q_l1, k = 6, bs = "cr") +
  s(sls_unit_q_l2, k = 6, bs = "cr") +
  s(n_week, by = n_ind) '

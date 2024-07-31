frmla = ' sls_unit_q ~ s(co_loc_i, bs = "re") + offset(promo_price_lift) +
                    columbusday + goodfriday + easter + fathersday.minus + fathersday +
                    laborday.minus + laborday + mardigras + memorialday.minus +
                    memorialday + mlkday + mothersday.minus + mothersday + presidentsday +
                    superbowl.minus + superbowl + thanksgiving.minus + thanksgiving +
                    cyber.monday + valentines.minus + valentines + stpatricks +
                    cincodemayo + julyfourth.minus + julyfourth + halloween.minus +
                    halloween + veteransday + christmas.minus + christmas + newyearsday.minus +
                    newyearsday + s(t_o_y, bs = "cc", k = 25) +
                    s(sls_unit_q_L3, k = 6, bs = "cr") +
                    s(sls_unit_q_L4, k = 6, bs = "cr") +
                    s(n_week, by = n_ind) '

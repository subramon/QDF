local function pr_jtype(jtype)
  if ( jtype ==   j_undef      ) then return "j_undef" end 
  if ( jtype ==   j_nil        ) then return "j_nil" end    
  if ( jtype ==   j_bool       ) then return "j_bool" end  
  if ( jtype ==   j_number     ) then return "j_number" end  
  if ( jtype ==   j_string     ) then return "j_string" end  
  if ( jtype ==   j_array      ) then return "j_array" end  
  if ( jtype ==   j_object     ) then return "j_object" end  
  error("control should not come here")
end
return pr_jtype

  // const char * const lbrry = "library(mgcv, lib.loc='/home/subramon/R/')";
  const char * const lbrry = "library(mgcv)";
  const char * const fnctn = 
    " callgam <- function(rsdf) { "
    " rsdf$co_loc_i <- as.factor(rsdf$co_loc_i); "
    " args <- list( "
    "   data =  rsdf, " 
    "   family = nb,  "
    "   formula = compiled_frmla, "
    "   controls = list(trace = FALSE, keepData = FALSE) "
    " ); "
    " m <- do.call(gam, args); "
    " coeff <- m$coefficients; "
    " return(coeff); "
    " } ";
  const char * const callR = "coeff <- callgam(rsdf) ";

#include "incs.h"
#include "utils_macros.h"
#include "qtypes.h"

qtype_t
get_tm_qtype(
    const char * const fld
    )
{
  if ( strcmp(fld, "tm_year") == 0 ) { return I2; }
  if ( strcmp(fld, "tm_mon")  == 0 ) { return I1; }
  if ( strcmp(fld, "tm_mday") == 0 ) { return I1; }
  if ( strcmp(fld, "tm_hour") == 0 ) { return I1; }
  // if ( strcmp(fld, "tm_min")  == 0 ) { return I1; }
  // if ( strcmp(fld, "tm_sec")  == 0 ) { return I1; }
  if ( strcmp(fld, "tm_wday") == 0 ) { return I1; }
  if ( strcmp(fld, "tm_yday") == 0 ) { return I2; }
  fprintf(stderr, "Bad tm fld = %s \n", fld);
  return 0; 
}

int
t_assign(
    struct tm *dst, 
    const tm_t * const src
    )
{
  int status = 0;
  if ( dst == NULL ) { go_BYE(-1); }
  if ( src == NULL ) {  go_BYE(-1); }
  memset(dst, 0, sizeof(struct tm));
  //  dst->tm_sec  = src->tm_sec;
  //  dst->tm_min  = src->tm_min;
  dst->tm_hour = src->tm_hour;
  dst->tm_mday = src->tm_mday;
  dst->tm_mon  = src->tm_mon;
  dst->tm_year = src->tm_year;
  dst->tm_yday = src->tm_yday;
BYE:
  return status;
}

uint32_t
x_get_width_qtype(
    char * const str_qtype
    )
{
  if ( str_qtype == NULL ) { WHEREAMI; return -1; }
  qtype_t qtype = get_c_qtype(str_qtype);
  uint32_t width = get_width_c_qtype(qtype);
  return width;
}

uint32_t
get_width_c_qtype(
      qtype_t qtype
    )
{
  switch ( qtype ) { 
    case BL : return sizeof(bool); break;

    case I1 : return sizeof(int8_t); break;
    case I2 : return sizeof(int16_t); break;
    case I4 : return sizeof(int32_t); break;
    case I8 : return sizeof(int64_t); break;

    case UI1 : return sizeof(uint8_t); break;
    case UI2 : return sizeof(uint16_t); break;
    case UI4 : return sizeof(uint32_t); break;
    case UI8 : return sizeof(uint64_t); break;

    case F2 : return sizeof(bfloat16); break;
    case F4 : return sizeof(float); break;
    case F8 : return sizeof(double); break;
    case TM : return sizeof(tm_t); break;
    case HL : return sizeof(holiday_bmask_t); break;
    default : return 0; break;
  }
}

qtype_t
get_c_qtype(
    const char *const qtype
    )
{
  if ( qtype == NULL ) { return Q0; }
  if ( strcmp("I1", qtype) == 0 ) { return I1; }
  if ( strcmp("I2", qtype) == 0 ) { return I2; }
  if ( strcmp("I4", qtype) == 0 ) { return I4; }
  if ( strcmp("I8", qtype) == 0 ) { return I8; }

  if ( strcmp("UI1", qtype) == 0 ) { return UI1; }
  if ( strcmp("UI2", qtype) == 0 ) { return UI2; }
  if ( strcmp("UI4", qtype) == 0 ) { return UI4; }
  if ( strcmp("UI8", qtype) == 0 ) { return UI8; }

  if ( strcmp("F4", qtype) == 0 ) { return F4; }
  if ( strcmp("F8", qtype) == 0 ) { return F8; }

  if ( strcmp("SC", qtype) == 0 ) { return   SC; }  
  if ( strncmp("SC:", qtype, 3) == 0 ) { return SC; }   // NOTE

  if ( strcmp("SV", qtype) == 0 ) { return SV; }  
  if ( strcmp("HL", qtype) == 0 ) { return HL; }  

  if ( strcmp("TM", qtype) == 0 ) { return TM; }  
  if ( strncmp("TM:", qtype, 3) == 0 ) { return TM; }  
  return Q0;
}
const char *
get_str_qtype(
    qtype_t qtype
    )
{
  if ( qtype == Q0 ) { return "Q0"; }
  if ( qtype == BL ) { return "BL"; }

  if ( qtype == I1 ) { return "I1"; }
  if ( qtype == I2 ) { return "I2"; }
  if ( qtype == I4 ) { return "I4"; }
  if ( qtype == I8 ) { return "I8"; }

  if ( qtype == UI1 ) { return "UI1"; }
  if ( qtype == UI2 ) { return "UI2"; }
  if ( qtype == UI4 ) { return "UI4"; }
  if ( qtype == UI8 ) { return "UI8"; }

  if ( qtype == F4 ) { return "F4"; }
  if ( qtype == F8 ) { return "F8"; }

  if ( qtype == SC ) { return "SC"; }
  if ( qtype == TM ) { return "TM"; }
  if ( qtype == HL ) { return "HL"; }
  return NULL; 
}


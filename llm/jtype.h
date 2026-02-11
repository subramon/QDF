#ifndef __JTYPE_H
#define __JTYPE_H
typedef enum { 
  j_error = 0, 
  j_undef,
  j_nil, 
  j_bool, 
  j_string,
  j_number, 
  j_date, 
  j_array, 
  j_object,
  // j_hashtable
} jtype_t;
#endif // __JTYPE_H

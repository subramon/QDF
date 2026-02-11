#ifndef __QTYPE_H
#define __QTYPE_H
typedef enum { 
  Qerr = 0,
  Q0,
  B1, // boolean stored as bit
  BL, // boolean stored as bool
  I8, // signed 1-byte intege
  I16, // signed 2-byte intege
  I32, // signed 4-byte intege
  I64, // signed 8-byte intege
  BF16, // bfloat16 floating point 
  FP32, // single precision floating point 
  FP64, // double prevision floating point 
  UI8, // unsigned 1-byte integer
  UI16, // unsigned 2-byte integer
  UI32, // unsigned 4-byte integer
  UI64, // unsigned 8-byte integer
  SC,  // consatnt length string 
  TM, // struct tm as defined in time.h
} qtype_t;
#endif __QTYPE_H

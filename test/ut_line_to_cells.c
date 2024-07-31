#include "incs.h"
#include "qtypes.h"
#include "auxil.h"
#include "line_to_cells.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  const char *str1 = "a,bc,def,ghij\n";
  int n1 = 4; int sz1 = 8;
  char **cells = NULL;
  int fld_sep = ','; int rec_sep = '\n';
  //----------------
  int *break_cols = NULL; int n_break_cols = 2;
  uint32_t *break_vals = NULL;
  //----------------
  if ( argc != 1 ) { go_BYE(-1); }


  cells = malloc(n1 * sizeof(char *));
  for ( int i = 0; i < n1; i++ ) { cells[i] = malloc(sz1); } 
  int len1 = strlen(str1);

  status = line_to_cells(str1, len1, n1, fld_sep, rec_sep,cells, sz1); 
  if ( strcmp(cells[0], "a") != 0 ) { go_BYE(-1); } 
  if ( strcmp(cells[1], "bc") != 0 ) { go_BYE(-1); } 
  if ( strcmp(cells[2], "def") != 0 ) { go_BYE(-1); } 
  if ( strcmp(cells[3], "ghij") != 0 ) { go_BYE(-1); } 
  cBYE(status);
  free_2d_array(&cells, n1); 
  //-------------------
  const char *str2 = "1,23,456,7890\n";
  int len2 = strlen(str2);
  int n2 = 4; int sz2 = 8;
  cells = malloc(n2 * sizeof(char *));
  for ( int i = 0; i < n2; i++ ) { cells[i] = malloc(sz2); } 
  status = line_to_cells(str2, len2, n2, fld_sep, rec_sep,cells, sz2); 
  cBYE(status);
  free_2d_array(&cells, n2); 

  fprintf(stdout, "Test %s completed successfully\n", argv[0]);

BYE:
  free_if_non_null(break_cols);
  free_if_non_null(break_vals);
  return status;
}

#include "incs.h"
#include "grouping.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  FILE *ofp = NULL;
  grouping_t *G = NULL;
  grouping_t *G2 = NULL;
  grouping_t *G3 = NULL;
  int *g = NULL;
  int nG = 0;
  int nG2 = 0;
  int nG3 = 0;
  int ng = 0;
  if ( argc != 4 ) { go_BYE(-1); }
  char *infile = argv[1];
  char *outfile = argv[2];
  char *infile2 = argv[3];
  if (strcmp(infile, outfile) == 0 ) { go_BYE(-1); }
  if (strcmp(infile2, outfile) == 0 ) { go_BYE(-1); }
  status = read_grouping(infile, "style", &G, &nG); cBYE(status);
  ofp = fopen(outfile, "w");
  for ( int i = 0; i < nG; i++ ) { 
    fprintf(ofp, "%d,%d,%d\n", 
        G[i].mdse_item_i, 
        G[i].dist_loc_i,
        G[i].mdse_styl_i);
  }
  status = test_grouping(G, nG); cBYE(status);
  //----------------------------
  status = read_grouping(infile2, NULL, &G2, &nG2); cBYE(status);
  for ( int i = 0; i < nG2; i++ ) { 
    G2[i].mdse_styl_i = 1; // just to pass the test 
  }
  status = test_grouping(G2, nG2); cBYE(status);
  //----------------------------
  status = set_groups(G, nG, G2, nG2); cBYE(status);
  status = uq_groups(G2, nG2, &g, &ng); cBYE(status);
  for ( int i = 0; i < ng; i++ ) { 
    printf("g[%d] = %d \n", i, g[i]);
  }
  // G3 is a subset of G where the mdse_styl_i is in g[]
  // It is sorted on mdse_styl_i
  status = extract_groupings(G, nG, g, ng, &G3, &nG3); cBYE(status);
  printf("Test %s completed successfully \n", argv[0]);
BYE:
  fclose_if_non_null(ofp);
  free_if_non_null(G);
  free_if_non_null(G2);
  free_if_non_null(G3);
  free_if_non_null(g);
  return status;
}

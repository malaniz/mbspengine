#include "mcbsp.h"
#include "mcbsp-affinity.h"
#include "mbspdebug.h"
#include "mbsptime.h"
#include "mbsputil.h"
#include "mbspdata.h"
#include "mbspdiscover.h"

#define N 1024

#define NDEBUG
#define MANUAL_AFFINITY 2


double*              data = NULL;


char* sarrayi(int v[], int length) 
{
  int i;
  char* s = (char*) malloc (sizeof(char)*length);

  sprintf(s, "[ ");
  for (i=0; i<length; i++) {
    sprintf(s, "%s %d ", s, v[i]);
  }
  sprintf(s, "%s ]", s);

  return s;
}
char* sarrayt(size_t v[], int length) 
{
  int i;
  char* s = (char*) malloc (sizeof(char)*length);

  sprintf(s, "[ ");
  for (i=0; i<length; i++) {
    sprintf(s, "%s %zu ", s, v[i]);
  }
  sprintf(s, "%s ]", s);

  return s;
}



double work(interval l)
{
  double  inprod = 0.0;
  int     i;
  for (i=l.start; i<l.end; i++) inprod += data[i]*data[i];
  return inprod;
}

double reduce ( double values[] , int n) 
{
  int     i;
  double  result;

  for (i=0; i<n; i++) result += values[i];
}

interval partition(interval l, int p, int n)
{
  interval newl;

  newl.start = (ILENGTH(l) / n) * p;
  newl.end   = (ILENGTH(l) / n) * (p+1);

  return newl;
}

int engine(interval l, multibsp_array_node_t t) 
{
  int                   i;
  int                   length;
  size_t                size = 10;
  size_t                pin[2] = { 0, 1} ;
    
  int                   n, p;
  double                r; 
  double*               rlevel;
  interval              interval;
  multibsp_array_node_t tnew;
  


  mcbsp_set_pinning( pin, 2);
  bsp_begin(bsp_nprocs());
  bsp_push_reg( t, sizeof(multibsp_array_node)*10 );
  bsp_push_reg( &l, sizeof(interval) );
  bsp_sync();
  if (bsp_pid() > 0) {
    bsp_direct_get(0, t, 0, t, sizeof(multibsp_array_node)*10);
    bsp_direct_get(0, &l, 0, &l, sizeof(interval));
  }
  bsp_sync();

  p      = bsp_pid();
  n      = bsp_nprocs();
  rlevel = vecallocd(n);
  r      = 0;
  tnew   = NULL;



  if (n > 0)  {
    interval  = partition(l, p, n);

    // TODO: definir next del arbol
    //tnew      = t->sons[p];
    bsp_init( &engine, 0, NULL ); 
    rlevel[p] = engine(interval, tnew);
    bsp_sync();
    // barrier y luego reducir
    rlevel[p] = reduce(rlevel, n);


    bsp_end();
  } else {

    r         = work(interval);
  }

  return r;

}

int main(int argc, char **argv)
{

  int         i = 0;
  double      r = 0;
  interval  l;

  multibsp_tree_node_t ttt = multibsp_discover_new();
  multibsp_array_node_t tree;

  tree = multibsp_array_new(10);

  tree = multibsp_to_array(ttt, tree);

  mcbsp_set_affinity_mode( MANUAL_AFFINITY ); // go MANUAL affinity

  data = vecallocd(N); for (i=0; i<N; i++) data[i] = 1;

  l.start = 0; l.end   = N;
  bsp_init( &engine, 0, NULL ); 
  r = engine(l, tree);
  log_info("Result: %f", r);

  // freeing memory
  free(data);
  free(tree);


  exit(0);

}

// ANOTATIONS
/*
int main(void) 
{
  multibsp_tree_node_t  t = multibsp_discover_new();
  multibsp_array_node_t v, l, f;
  int* k;

  v = multibsp_array_new(10);

  v = multibsp_to_array(t, v);
 // f = multibsp_get_level(v, 0);
  //l = multibsp_get_level_with_id(v, 0, 1);
  k = multibsp_get_index_with_level_id(v, 0, 1);



  log_info("t: %s", multibsp_discover_print(t));
  log_info("v: %s", multibsp_array_print(v));
  log_info("k: %s", sarrayi(k, 2));
  
  return 0;
}
*/



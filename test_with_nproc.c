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

double engine(int pos, int tree[]) 
{
  int threads = 4;
  size_t pin[4];
  size_t size = 10;
  pin[0] = tree[pos+1] % size;
  pin[1] = tree[pos+2] % size;
  pin[2] = tree[pos+3] % size;
  pin[3] = tree[pos+4] % size;



  mcbsp_set_pinning(pin, threads);

  bsp_begin(bsp_nprocs());
  log_info("HERE THE PROBLEM<pos: %d, pid: %d, pin: %s>", pos, bsp_pid(), sarrayt((int*)pin, 4));
  bsp_push_reg(&pos, SZDBL);
  bsp_push_reg(tree, SZINT*7);
  bsp_sync();
  if (bsp_pid() > 0) {
    bsp_direct_get(0, &pos, 0, &pos, SZINT);
    bsp_direct_get(0, tree, 0, tree, SZINT*7);
  }
  bsp_sync();
  //log_info("<%d %d %d %d> -- %s", tree[pos+1], tree[pos+2], tree[pos+3], tree[pos+4], sarrayi(tree, 7));

  pin[0] = tree[pos+1] % size;
  pin[1] = tree[pos+2] % size;
  pin[2] = tree[pos+3] % size;
  pin[3] = tree[pos+4] % size;


  log_info("<pos: %d, pid: %d, pin: %s>", pos, bsp_pid(), sarrayt(pin, 4));

  bsp_end();
  return 0.0;
  
}


int main(int argc, char **argv)
{

  double      r = 0;
  double*     rlevel;
  int         vtree[] = {0, 0, 2, 3, 1, 2, 3 };

  mcbsp_set_affinity_mode( MANUAL_AFFINITY ); 
  bsp_init( &engine, 0, NULL );
  r = engine(0, vtree); 



  exit(0);

} /* end main */


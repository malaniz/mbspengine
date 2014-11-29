#include "mcbsp.h"
#include "mcbsp-affinity.h"
#include "mbspdebug.h"
#include "mbsptime.h"
#include "mbsputil.h"
#include "mbspdata.h"
#include "mbspdiscover.h"

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

double engine(multibsp_array_node_t tree, int level, int parentid) 
{

  size_t* pin = (size_t*) malloc (sizeof(size_t)*2);

  bsp_begin(bsp_nprocs());

  if (bsp_pid() > 0) {
    tree = multibsp_array_new(10);
  }else {
  }

  pin[0] = 0; pin[1] = 1;
  bsp_push_reg(tree, sizeof(multibsp_array_node)*10);
  bsp_push_reg(&level, SZINT);
  bsp_push_reg(&parentid, SZINT);
  bsp_sync();

  if (bsp_pid() > 0) {
    bsp_direct_get(0, tree , 0, tree, sizeof(multibsp_array_node)*10);
    bsp_direct_get(0, &level, 0, &level, SZINT);
    bsp_direct_get(0, &parentid, 0, &parentid, SZINT);
  }

  bsp_sync();

  parentid = bsp_pid();

  if (level == 0) {
    log_info("level:%d - leaf <n: %d, p: %d> - %s", 
        level, bsp_nprocs(), bsp_pid(), multibsp_array_print(tree)
    );
  } else {

    //log_info("level:%d - node ", level);

    log_info("level: %d, parentid: %d, pin: %s", level, parentid, sarrayt(pin,2));
    level--;
    bsp_sync();
    bsp_init( &engine, 0, NULL);
    mcbsp_set_pinning(pin, 2);
    engine(tree, level, parentid);
  }

  bsp_sync();

  bsp_end();
  return 0.0;
}


int main(int argc, char **argv)
{

  double      r = 0;
  double*     rlevel;
  multibsp_tree_node_t ttt = multibsp_discover_new();
  multibsp_array_node_t t;
  t = multibsp_array_new(10);
  t = multibsp_to_array(ttt, t);

  mcbsp_set_affinity_mode( MANUAL_AFFINITY ); 
  bsp_init( &engine, 0, NULL );

  size_t* pin = (size_t*) malloc (sizeof(size_t)*2);
  pin[0] = 0; pin[1] = 2;

  log_info("level: %d, parentid: %d, pin: %s", 2, 0, sarrayt(pin,2));
  mcbsp_set_pinning(pin, 2);
  engine(t, 1, 0); 


  exit(0);

} /* end main */



#include "mcbsp.h"
#include "mcbsp-affinity.h"
#include "mbspdebug.h"
#include "mbsptime.h"
#include "mbsputil.h"
#include "mbspdata.h"
#include "mbspdiscover.h"

#define NDEBUG

#define N 10
#define INVALID_NODE -1
typedef struct {
  int index;
  int next;
} multibsp_array_node;

typedef multibsp_array_node* multibsp_array_node_t;

int position_array = 0;

multibsp_array_node_t multibsp_array_new(int size)
{
  int i;
  multibsp_array_node_t arr = (multibsp_array_node_t) malloc (sizeof(multibsp_array_node)*size);
  for (i=0; i<size; i++) {
    arr[i].index = INVALID_NODE;
    arr[i].next  = INVALID_NODE;
  }
  return arr;
}


multibsp_array_node_t multibsp_to_array(multibsp_tree_node_t t, multibsp_array_node_t arr) 
{
  int i, cur;
  arr[position_array++].index = t->index;
  if ( t->length > 0 ) {
    for ( i=0; i<t->length; i++ ) {
      cur = position_array;
      multibsp_to_array(t->sons[i], arr);
      if ( i == t->length -1) arr[cur].next = -1;
      else                    arr[cur].next = position_array;
    }
  }
  return arr;
}

multibsp_array_node_t multibsp_get_level(multibsp_array_node_t arr, int level) 
{
  int cur_level = 0;
  int position = 0;
  int i;
  multibsp_array_node res[50];
  multibsp_array_node_t result;

  for (i=0; i<level; i++){
    if (arr[i].next == INVALID_NODE)
      cur_level++;
  } 
  position = i;
  i = 0;
  do {
    res[i].index = arr[position].index;
    res[i].next = arr[position].next;
    position++; i++;
  }
  while(arr[position].next != INVALID_NODE);


  result = (multibsp_array_node_t) malloc (sizeof(multibsp_array_node)*i);

  position = i;
  for (i=0; i<position; i++) {
    result[i].index = res[i].index;
    result[i].next  = res[i].next ;
  }

  return result;
}

char* multibsp_array_print(multibsp_array_node_t arr) {
  int i;
  char* str = (char*) malloc (sizeof(char)*N);
  sprintf(str, "[ ");
  for (i=0; i<N; i++) {
    sprintf(str, "%s \t<%d, %d>\t", str, arr[i].index, arr[i].next);
  }
  sprintf(str, "%s ]", str);

  return str;

}

int main(int argc, char **argv)
{
  int i;
  multibsp_tree_node_t tree;
  tree = multibsp_discover_new();

  multibsp_array_node_t arr = multibsp_array_new(N);
  arr = multibsp_to_array(tree, arr);
  
  arr = multibsp_get_level(arr, 1);

  log_info("%s", multibsp_discover_print(tree));
  log_info("%s", multibsp_array_print(arr));

  exit(0);

} 






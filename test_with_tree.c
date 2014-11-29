#include "mcbsp.h"
#include "mcbsp-affinity.h"
#include "mbspdebug.h"
#include "mbsptime.h"
#include "mbsputil.h"
#include "mbspdata.h"
#include "mbspdiscover.h"

#define NDEBUG

/*
int position_array = 0;

multibsp_array_node_t multibsp_array_new(int size)
{
  int i;
  multibsp_array_node_t arr = (multibsp_array_node_t) malloc (sizeof(multibsp_array_node)*size);
  for (i=0; i<size; i++) {
    arr[i].index = INVALID_NODE;
    arr[i].level = INVALID_NODE;
    arr[i].next  = INVALID_NODE;
  }
  return arr;
}


char* multibsp_array_print(multibsp_array_node_t arr) {
  int i, size;

  for (size=0; arr[size].index != INVALID_NODE; size++) ;
  char* str = (char*) malloc (sizeof(char)*size*10);
  sprintf(str, "[ ");
  for (i=0; i<size; i++) {
    sprintf(str, "%s <%d, %d, %d>", str, arr[i].index, arr[i].next, arr[i].level);
  }
  sprintf(str, "%s ]", str);

  return str;

}

multibsp_array_node_t multibsp_to_array(multibsp_tree_node_t t, multibsp_array_node_t arr) 
{
  int i, cur;
  arr[position_array].index = t->index;
  arr[position_array].level = t->level;
  
  position_array++;
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
  multibsp_array_node_t res;
  int size, i, j;
  
  size=0; 
  for(i=0; arr[i].index!=INVALID_NODE; i++) if(arr[i].level==level) 
    size++;

  res = multibsp_array_new(size);

  for(j=0, i=0; arr[i].index!=INVALID_NODE; i++) {
    if (arr[i].level==level) {
      res[j].level = arr[i].level;
      res[j].index = arr[i].index;
      res[j].next  = arr[i].next;
      j++;
    }
  }
  res[j].index = INVALID_NODE;
  res[j].level = INVALID_NODE;
  res[j].next  = INVALID_NODE;

  return res;
}


multibsp_array_node_t multibsp_get_level_with_id(multibsp_array_node_t arr, int level, int id) 
{
  int curid, i, j, k;
  multibsp_array_node_t res, resid;
  res = multibsp_get_level(arr, level);


  curid=0;
  j=0;
  for (i=0; res[i].index != INVALID_NODE; i++) {
    if(curid > id) break;
    if(curid == id) {
      resid[j].index = res[i].index;
      resid[j].level = res[i].level;
      resid[j].next  = res[i].next;
      j++;
    }
    if (res[i].next == INVALID_NODE) curid++;
  }
  resid[j].index = INVALID_NODE;
  resid[j].level = INVALID_NODE;
  resid[j].next  = INVALID_NODE;
  return resid;
}

*/
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


int main(int argc, char **argv)
{
  int i;
  int n, l ,p;
  int size2;
  size_t *pin;
  multibsp_tree_node_t tree;
  tree = multibsp_discover_new();

  multibsp_array_node_t arr = multibsp_array_new(10);
  multibsp_array_node_t arr2;
  multibsp_array_node_t arr3;

  arr = multibsp_to_array(tree, arr);
  
  log_info("%s", multibsp_discover_print(tree));
  log_info("%s", multibsp_array_print(arr));

  n = 2;
  l = 0;
  p = 0;
  pin = multibsp_get_index_with_level_id(arr, l, p);
  log_info("%d, %d - %s", l, p, sarrayt(pin, n));
  n = 2;
  l = 0;
  p = 1;
  pin = multibsp_get_index_with_level_id(arr, l, p);
  log_info("%d, %d - %s", l, p, sarrayt(pin, n));

  exit(0);
} 


















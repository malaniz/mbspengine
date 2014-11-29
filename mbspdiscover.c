#include <hwloc.h>
#include "mbspdiscover.h"
#include "mbspdebug.h"

multibsp_tree_node_t make_mapping(hwloc_topology_t topology, hwloc_obj_t obj, int depth )
{
    char string[128];
    unsigned i;
    multibsp_tree_node_t n = NULL;
    multibsp_tree_node_t ntemp = NULL;

    if (obj->arity == 1) {
      // nodes with arity 1 are unnecesary for mapping 
      n = make_mapping(topology, obj->children[0], depth + 1);

    } else { // start: needed nodes

      n = (multibsp_tree_node_t) malloc(sizeof(multibsp_tree_node));

      n->length = obj->arity;
      hwloc_obj_snprintf(string, sizeof(string), topology, obj, "#", 0);
      sprintf(n->description, "%*s%s\n", 2*depth, "", string);


      // enter in to recursion
      for (i = 0; i < n->length; i++) {
        n->sons[i] = make_mapping(topology, obj->children[i], depth + 1);
        n->sons[i]->parent = n;
      }

      // back from recursion
      if (n->length == 0) {
        n->index = obj->os_index;
        n->level = 0;
      } else {
        n->index = n->sons[0]->index;
        n->level = n->sons[0]->level+ 1;
      }
    } // end: needed nodes
    return n;
}

multibsp_tree_node_t multibsp_discover_new()
{
  multibsp_tree_node_t mapping = NULL;


  // start: HWLOC detection code 
  hwloc_topology_t  topology;

  hwloc_topology_init(&topology);
  hwloc_topology_ignore_type(topology, HWLOC_OBJ_SOCKET);
  hwloc_topology_load(topology);
  // end: hwloc detection code 


  mapping = make_mapping(topology, hwloc_get_root_obj(topology), 0);
  return mapping;

}


char* multibsp_discover_print (multibsp_tree_node_t n) 
{
  char* result = (char*) malloc (sizeof(char)*4096);
  int i;

  if (n->length == 0 ){
    sprintf(result, "%d", n->index);
  } else {
    sprintf(result, "%d [ ", n->index);
    for (i=0; i< n->length; i++) {
      sprintf(result, "%s %s ", result, multibsp_discover_print(n->sons[i]));
    }
    sprintf(result, "%s ]", result);
  }
  return result;
}



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
  char* str = (char*) malloc (sizeof(char)*size*20);
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

multibsp_array_node_t 
multibsp_get_level(multibsp_array_node_t arr, int level) 
{
  multibsp_array_node_t res;
  int size, i, j;
  
  size=0; 
  for(i=0; arr[i].index!=INVALID_NODE; i++) if(arr[i].level==level) 
    size++;

  res = multibsp_array_new(size+1);

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

  j=0;
  curid=0;
  for (i=0; res[i].index != INVALID_NODE; i++) {
    if (curid >  id) break;
    if (curid == id) j++;
    if (res[i].next == INVALID_NODE) curid++;
  }

  resid = multibsp_array_new(j);
  
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

size_t* multibsp_get_index_with_level_id(multibsp_array_node_t arr, int level, int id) 
{
  int curid, i, j, k;
  multibsp_array_node_t res ;
  size_t Z = 10;
  size_t* resid;
  
  res = multibsp_get_level(arr, level);
  
  j=0;
  curid=0;
  for (i=0; res[i].index != INVALID_NODE; i++) {
    if (curid >  id) break;
    if (curid == id) j++;
    if (res[i].next == INVALID_NODE) curid++;
  }


  resid = (size_t*) malloc (sizeof(size_t)*j);

  curid=0;
  j=0;
  for (i=0; res[i].index != INVALID_NODE; i++) {
    if(curid > id) break;
    if(curid == id) {
      resid[j] = res[i].index % Z;
      j++;
    }
    if (res[i].next == INVALID_NODE) curid++;
  }
  return resid;
}



#ifdef DISCOVER_DEBUG
int main () {
  char* strp;
  multibsp_tree_node_t tree = NULL;
  tree = multibsp_discover_new();
  strp = multibsp_discover_print(tree);
  printf("%s\n", strp);
  return 0;
}
#endif

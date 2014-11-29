/* start: TREE Structure module */
typedef struct multibsp_tree_node {
  char                          description[128];
  struct multibsp_tree_node*    sons[100];
  struct multibsp_tree_node*    parent;
  int                           index;
  int                           length;
  int                           level; // if length == 0 then is a leaf
  char                          type[128];
} multibsp_tree_node;

typedef multibsp_tree_node* multibsp_tree_node_t;


// start: build mapping structure <<constructor>>
multibsp_tree_node_t multibsp_discover_new();
// end: build mapping structure 

// start: print mapping struct  
char* multibsp_discover_print(multibsp_tree_node_t n);
// end: print mapping struct  



// start: array structure 

#define INVALID_NODE -1
typedef struct {
  int index;
  int next;
  int level;
} multibsp_array_node;

// end: array structure 

typedef multibsp_array_node* multibsp_array_node_t;

// start: functions for serialize the tree in array
multibsp_array_node_t multibsp_array_new(int size);
char* multibsp_array_print(multibsp_array_node_t arr);
multibsp_array_node_t multibsp_to_array(multibsp_tree_node_t , multibsp_array_node_t ) ;
multibsp_array_node_t multibsp_get_level(multibsp_array_node_t , int ) ;
multibsp_array_node_t multibsp_get_level_with_id(multibsp_array_node_t , int , int ) ;
size_t*  multibsp_get_index_with_level_id(multibsp_array_node_t, int, int);
// end : functions for serialize the tree in array


/* end: TREE Structure module */

#ifndef ASH_ENVIRONMENT_DEFINED
#define ASH_ENVIRONMENT_DEFINED
struct NodeStruct;

typedef struct Environment {
  int    count;
  char** symbols;
  struct NodeStruct** nodes;
} Environment;

typedef struct NodeStruct* (*BuiltIn)(Environment*, struct NodeStruct*);

Environment* new_environment(void);
void environment_delete(Environment*);
struct NodeStruct* environment_get(Environment*, struct NodeStruct*);
void environment_put(Environment*, struct NodeStruct*, struct NodeStruct*);
void environment_add_builtins(Environment* e);
#endif

#ifndef ENVIRONMENT_DEFINED
#define ENVIRONMENT_DEFINED
struct NodeStruct;

typedef struct Environment {
  int    count;
  char** symbols;
  struct NodeStruct** nodes;
} Environment;

typedef struct NodeStruct* (*BuiltIn)(Environment*, struct NodeStruct*);

Environment* new__Environment(void);
void Environment__free(Environment*);
struct NodeStruct* Environment__get(Environment*, struct NodeStruct*);
void Environment__put(Environment*, struct NodeStruct*, struct NodeStruct*);
void Environment__add_builtins(Environment* e);
#endif

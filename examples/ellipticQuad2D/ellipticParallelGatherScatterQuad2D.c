#include "ellipticQuad2D.h"

void ellipticParallelGatherScatterQuad2D(mesh2D *mesh, ogs_t *ogs, occa::memory &o_q, const char *type, const char *op){

  // use gather map for gather and scatter
  occaTimerTic(mesh->device,"meshParallelGatherScatter2D");
  meshParallelGatherScatter(mesh, ogs, o_q);
  occaTimerToc(mesh->device,"meshParallelGatherScatter2D"); 
}
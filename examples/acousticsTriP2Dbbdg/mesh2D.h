#ifndef MESH2D_H 
#define MESH2D_H 1

#include "mesh.h"

// will eventually rename mesh2D to mesh_t in src
#define mesh2D mesh_t

mesh2D* meshReaderTri2D(char *fileName);
mesh2D* meshReaderQuad2D(char *fileName);

// mesh readers
mesh2D* meshParallelReaderTri2D(char *fileName);
mesh2D* meshParallelReaderQuad2D(char *fileName);

// build connectivity in serial
void meshConnect2D(mesh2D *mesh);

// build element-boundary connectivity
void meshConnectBoundary2D(mesh2D *mesh);

// build connectivity in parallel
void meshParallelConnect2D(mesh2D *mesh);

// build global connectivity in parallel
void meshParallelConnectNodesQuad2D(mesh2D *mesh);

// create global number of nodes
void meshNumberNodes2D(mesh2D *mesh);

// repartition elements in parallel
void meshGeometricPartition2D(mesh2D *mesh);

// print out mesh 
void meshPrint2D(mesh2D *mesh);

// print out mesh in parallel from the root process
void meshParallelPrint2D(mesh2D *mesh);

// print out mesh partition in parallel
void meshVTU2D(mesh2D *mesh, char *fileName);

// print out solution at plot nodes 
void meshPlotVTU2DP(mesh2D *mesh, char *fileNameBase, iint fld);

// sort entries in an array in parallel
void parallelSort(iint N, void *vv, size_t sz,
		  int (*compare)(const void *, const void *),
		  void (*match)(void *, void *)
		  );

// compute geometric factors for local to physical map 
void meshGeometricFactorsTri2D(mesh2D *mesh);
void meshGeometricFactorsQuad2D(mesh2D *mesh);

void meshSurfaceGeometricFactorsTriP2D(mesh2D *mesh);
void meshSurfaceGeometricFactorsQuad2D(mesh2D *mesh);

void meshPhysicalNodesTriP2D(mesh2D *mesh);
void meshPhysicalNodesQuad2D(mesh2D *mesh);

void meshLoadReferenceNodesTriP2D(mesh2D *mesh, int N);
void meshLoadReferenceNodesQuad2D(mesh2D *mesh, int N);

void meshGradientTri2D(mesh2D *mesh, dfloat *q, dfloat *dqdx, dfloat *dqdy);
void meshGradientQuad2D(mesh2D *mesh, dfloat *q, dfloat *dqdx, dfloat *dqdy);

// print out parallel partition i
void meshPartitionStatistics2D(mesh2D *mesh);

// functions that call OCCA kernels
void occaTest(mesh2D *mesh);

// 
void occaOptimizeGradientTri2D(mesh2D *mesh, dfloat *q, dfloat *dqdx, dfloat *dqdy);
void occaOptimizeGradientQuad2D(mesh2D *mesh, dfloat *q, dfloat *dqdx, dfloat *dqdy);

// serial face-node to face-node connection
void meshConnectFaceNodesP2D(mesh2D *mesh);

// halo connectivity information
void meshHaloSetup2D(mesh2D *mesh);

// perform complete halo exchange
void meshHaloExchange2D(mesh2D *mesh,
			size_t Nbytes, // number of bytes per element
			void *sourceBuffer, 
			void *sendBuffer, 
			void *recvBuffer);

// start halo exchange
void meshHaloExchangeStart2D(mesh2D *mesh,
			     size_t Nbytes,       // message size per element
			     void *sendBuffer,    // outgoing halo
			     void *recvBuffer);   // incoming halo

// finish halo exchange
void meshHaloExchangeFinish2D(mesh2D *mesh);

// extract halo data from sourceBuffer and save to sendBuffer
void meshHaloExtract2D(mesh2D *mesh, size_t Nbytes, void *sourceBuffer, void *sendBuffer);

// build list of nodes on each face of the reference element
void meshBuildFaceNodesTri2D(mesh2D *mesh);
void meshBuildFaceNodesQuad2D(mesh2D *mesh);

mesh2D *meshSetupTriP2D(char *filename, int N);
mesh2D *meshSetupQuad2D(char *filename, int N);


// set up OCCA device and copy generic element info to device
void meshOccaSetup2D(mesh2D *mesh, char *deviceConfig, occa::kernelInfo &kernelInfo);


// compute solution to cavity problem
void acousticsCavitySolution2D(dfloat x, dfloat y, dfloat time, 
			       dfloat *u, dfloat *v, dfloat *p);

// initial Gaussian pulse
void acousticsGaussianPulse2D(dfloat x, dfloat y, dfloat t,
			      dfloat *u, dfloat *v, dfloat *p);

void meshMRABSetupP2D(mesh2D *mesh, dfloat *EToDT, int maxLevels); 

//MRAB weighted mesh partitioning
void meshMRABWeightedPartitionTriP2D(mesh2D *mesh, dfloat *weights,
                                      iint numLevels, iint *levels);

void meshSetPolynomialDegree2D(mesh2D *mesh, int N);

#define norm(a,b) ( sqrt((a)*(a)+(b)*(b)) )

/* offsets for geometric factors */
#define RXID 0  
#define RYID 1  
#define SXID 2  
#define SYID 3  
#define  JID 4
#define JWID 5


/* offsets for second order geometric factors */
#define G00ID 0  
#define G01ID 1  
#define G11ID 2
#define GWJID 3

/* offsets for nx, ny, sJ, 1/J */
#define NXID 0  
#define NYID 1  
#define SJID 2  
#define IJID 3  
#define WSJID 4
#define IHID 5

#endif


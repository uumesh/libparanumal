/*

The MIT License (MIT)

Copyright (c) 2017-2022 Tim Warburton, Noel Chalmers, Jesse Chan, Ali Karakus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "mesh.hpp"

namespace libp {

// set up halo infomation for inter-processor MPI
// exchange of elements or trace nodes
void mesh_t::HaloSetup(){

  memory<hlong> globalOffset(size+1, 0);

  //gather number of elements on each rank
  hlong localNelements = Nelements;
  comm.Allgather(localNelements, globalOffset+1);

  for(int rr=0;rr<size;++rr)
    globalOffset[rr+1] = globalOffset[rr]+globalOffset[rr+1];

  // count number of halo element nodes to swap
  totalHaloPairs = 0;
  for(dlong e=0;e<Nelements;++e){
    for(int f=0;f<Nfaces;++f){
      int rr = EToP[e*Nfaces+f]; // rank of neighbor
      if(rr!=-1){
        totalHaloPairs++;
      }
    }
  }

  // count elements that contribute to a global halo exchange
  NhaloElements = 0;
  for(dlong e=0;e<Nelements;++e){
    for(int f=0;f<Nfaces;++f){
      int rr = EToP[e*Nfaces+f]; // rank of neighbor
      if(rr!=-1){
        NhaloElements++;
        break;
      }
    }
  }
  NinternalElements = Nelements - NhaloElements;

  //record the halo and non-halo element ids
  internalElementIds.malloc(NinternalElements);
  haloElementIds.malloc(NhaloElements);

  NhaloElements = 0, NinternalElements = 0;
  for(dlong e=0;e<Nelements;++e){
    int haloFlag = 0;
    for(int f=0;f<Nfaces;++f){
      int rr = EToP[e*Nfaces+f]; // rank of neighbor
      if(rr!=-1){
        haloFlag = 1;
        haloElementIds[NhaloElements++] = e;
        break;
      }
    }
    if (!haloFlag)
      internalElementIds[NinternalElements++] = e;
  }

  // Send to device
  o_internalElementIds = platform.malloc<dlong>(internalElementIds);
  o_haloElementIds = platform.malloc<dlong>(haloElementIds);

  //make a list of global element ids taking part in the halo exchange
  memory<hlong> globalElementId(Nelements+totalHaloPairs);

  //outgoing elements
  for(int e=0;e<Nelements;++e)
    globalElementId[e] = e + globalOffset[rank] + 1;

  //incoming elements
  totalHaloPairs = 0;
  for(dlong e=0;e<Nelements;++e){
    for(int f=0;f<Nfaces;++f){
      int rr = EToP[e*Nfaces+f]; // rank of neighbor
      if(rr!=-1){
        //EToE contains the local element number of the neighbor on rank rr
        globalElementId[Nelements+totalHaloPairs]
                       = -(EToE[e*Nfaces+f] + globalOffset[rr] + 1); //negative so doesnt contribute to sum in ogs

        // overwrite EToE to point to halo region now
        EToE[e*Nfaces+f] = Nelements+totalHaloPairs++;
      }
    }
  }

  //make a halo exchange op
  bool verbose = false;
  halo.Setup(Nelements+totalHaloPairs,
             globalElementId, comm,
             ogs::Pairwise, verbose, platform);

}

} //namespace libp


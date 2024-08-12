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

void mesh_t::Setup(platform_t& _platform, meshSettings_t& _settings,
                   comm_t _comm){

  platform = _platform;
  settings = _settings;
  props = platform.props();

  comm = _comm.Dup();
  rank = comm.rank();
  size = comm.size();

  int eType=0;
  settings.getSetting("ELEMENT TYPE", eType);
  settings.getSetting("MESH DIMENSION", dim);


  SetElementType(Mesh::ElementType(eType));

  props["defines/" "p_dim"]= dim;
  props["defines/" "p_Nfaces"]= Nfaces;

  std::string fileName;
  settings.getSetting("MESH FILE", fileName);

  if (settings.compareSetting("MESH FILE","PMLBOX")) {
    //build a box mesh with a pml layer
    SetupPmlBox();
  } else if (settings.compareSetting("MESH FILE","BOX")) {
    //build a box mesh
    SetupBox();
  } else {
    // read chunk of elements from file
    ReadGmsh(fileName);

    // partition elements using parAdogs
    Partition();
  }

  // load reference (r,s) element nodes
  settings.getSetting("POLYNOMIAL DEGREE", N);
  ReferenceNodes();

  // connect elements
  Connect();

  // connect elements to boundary faces
  ConnectBoundary();

  // set up halo exchange info for MPI (do before connect face nodes)
  HaloSetup();

  // connect face vertices
  ConnectFaceVertices();

  // connect face nodes
  ConnectFaceNodes();

  // make global indexing
  ConnectNodes();

  // compute physical (x,y) locations of the element nodes
  PhysicalNodes();

  // compute geometric factors
  GeometricFactors();

  // compute surface geofacs
  SurfaceGeometricFactors();

  // label local/global gather elements
  GatherScatterSetup();
}

} //namespace libp

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

#define PI 3.14159265358979323846

/* forcing function   */
#define ellipticForcing3D(x, y, z, lambda, f)  \
  {                                         \
    f  = (3*PI*PI+lambda)*sin(PI*x)*sin(PI*y)*sin(PI*z);   \
  }

/* Dirichlet boundary condition   */
#define ellipticDirichletCondition3D(x,y,z,nx,ny,nz,uM,uxM,uyM,uzM,uB,uxB,uyB,uzB)  \
  {              \
    uB  = sin(PI*x)*sin(PI*y)*sin(PI*z);   \
    uxB = uxM;   \
    uyB = uyM;   \
    uzB = uzM;   \
  }

/* Neumann boundary condition   */
#define ellipticNeumannCondition3D(x,y,z,nx,ny,nz,uM,uxM,uyM,uzM,uB,uxB,uyB,uzB)  \
  {              \
    uB  = uM;    \
    uxB = -PI*cos(PI*x)*sin(PI*y)*sin(PI*z);   \
    uyB = -PI*sin(PI*x)*cos(PI*y)*sin(PI*z);   \
    uzB = -PI*sin(PI*x)*sin(PI*y)*cos(PI*z);   \
  }

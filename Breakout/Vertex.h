

#ifndef VERTEX_H
#define VERTEX_H

//#include "d3dUtil.h"
#include "d3dUtil.h"


struct Vertex
{
    D3DXVECTOR3 pos;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 texC;
};

struct TreeVertex
{
	D3DXVECTOR3 centerW;
	D3DXVECTOR2 sizeW;
};
 
#endif // VERTEX_H
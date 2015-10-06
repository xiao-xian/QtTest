#include "ObjMesh.h"

#include <stdio.h>
#include <iostream>
using namespace std;

ObjMesh::ObjMesh( const string& fileName )
{
    loadObj( fileName );
}

void ObjMesh::setFile(const string &fileName)
{
    loadObj( fileName );
}

void ObjMesh::loadObj(const string &fileName)
{
    FILE * file = fopen( fileName.c_str(), "r");
    if( file == NULL )
    {
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
    }

    while( 1 )
    {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        if ( strcmp( lineHeader, "v" ) == 0 )
        {
            float x, y, z;
            fscanf(file, "%f %f %f\n", &x, &y, &z );

            double scale = 2;
            m_vertices.push_back( Vec3x( scale*x, scale*y, scale*z ));

        }
        else if(  strcmp( lineHeader, "f" ) == 0 )
        {
            //assuming it's triangles only
            int ix, iy, iz;
            fscanf(file, "%d %d %d\n", &ix, &iy, &iz );
            vector< int > indices;
            indices.push_back( ix - 1 );
            indices.push_back( iy - 1 );
            indices.push_back( iz - 1 );
            m_topology.push_back( indices );
        }
        else
        {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    std::vector< std::vector< Vec3x > > faceNormalsPerVertex;
    faceNormalsPerVertex.resize( m_vertices.size() );

    //go through each face and work out the normal
    for( size_t i = 0, num = m_topology.size(); i < num; i++ )
    {
        const std::vector< int >& indices = m_topology[ i ];
        const Vec3x v10 = ( m_vertices[ indices[ 1 ] ] - m_vertices[ indices[ 0 ] ] ).normalized();
        const Vec3x v20 = ( m_vertices[ indices[ 2 ] ] - m_vertices[ indices[ 0 ] ] ).normalized();
        const Vec3x normal = Vec3x::crossProduct( v20, v10 );

        // push
        for( size_t ii = 0; ii < indices.size(); ii++ )
        {
            faceNormalsPerVertex[ indices[ ii ] ].push_back( normal );
        }
    }

    //normalize
    for( size_t i = 0, num = faceNormalsPerVertex.size(); i < num; i++ )
    {
        const vector< Vec3x >& nList = faceNormalsPerVertex[ i ];
        Vec3x normal = nList[ 0 ];
        for( size_t ii = 1; ii < nList.size(); ii++ )
        {
            normal += nList[ ii ];
        }

        normal /= double( nList.size() );

        //normal = Vec3x( 1, 0, 0 );
        m_normals.push_back( normal );
    }

    for( int i = 0, num = m_topology.size(); i < num; i++ )
    {
        const std::vector< int >& id = m_topology[ i ];

        for( int ii = 0; ii < id.size(); ii++ )
        {
            Vec3x p = m_vertices[ id[ ii ] ];
            Vec3x n = m_normals[ id[ ii ] ];

            m_glData.push_back( p.x() );
            m_glData.push_back( p.y() );
            m_glData.push_back( p.z() );

            m_glData.push_back( n.x() );
            m_glData.push_back( n.y() );
            m_glData.push_back( n.z() );

        }

    }
}


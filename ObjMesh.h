#ifndef OBJMESH_H
#define OBJMESH_H

#include <qopengl.h>
#include <QVector3D>

#include <string>
#include <vector>

//when swtich to other app front, it's eaiser to just replace following typedefs:
typedef QVector3D Vec3x;

class ObjMesh
{
public:
    ObjMesh(){}
    explicit ObjMesh( const std::string& fileName );
    void setFile( const std::string& fileName );
    const GLfloat *constData() const { return m_glData.data(); }
    int count() const { return m_glData.size(); }
    int vertexCount() const { return m_glData.size() / 6; }

private:
    void loadObj( const std::string& fileName );

private:

    std::vector< Vec3x >                m_vertices; //raw positions
    std::vector< std::vector< int > >   m_topology; //indices per each polygon
    std::vector< Vec3x >                m_normals;  //worked out from the above two
    std::vector< GLfloat >              m_glData;   //for gl draw

};

#endif // OBJMESH_H

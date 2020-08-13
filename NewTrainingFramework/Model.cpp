#include "stdafx.h"
#include "Model.h"

enum MType
{
	NONE,
	NRVERTICES,
	NRINDICES,
	END
};

Model::Model()
{
	m_hIndexBuffer = 0;
	m_hVertexBuffer = 0;
	m_noIndices = 0;
	m_id = 0;	
}
Model::~Model()
{
	if(m_hVertexBuffer != NULL)
	{
		glDeleteBuffers(1,&m_hVertexBuffer);
		m_hVertexBuffer = NULL;
	}
	if(m_hIndexBuffer != NULL)
	{
		glDeleteBuffers(1,&m_hIndexBuffer);
		m_hIndexBuffer = NULL;
	}
}

bool Model::InitModel(char* filename)
{
    esLogMessage("Loading MODEL file --> %s <--...\n", filename);

    Vector3 temp_vertices;
    Vector3 temp_normals;
    Vector3 temp_binormals;
    Vector3 temp_tgt;
    Vector2 temp_uvs;
    Vector3 temp_indices;

    Vertex *bufferVertex = NULL;
    int *bufferIndices = NULL;

    unsigned int maxVertices = 0;
    unsigned int maxIndices = 0;
   
    FILE * pFile;
    errno_t errorCode;
    MType type = NONE;
    unsigned int cont = 0;    

    if((errorCode = fopen_s(&pFile, filename, "r"))!=0)
    {
        esLogMessage("Impossible to open the MODEL file !\n");
        return false;
    }
    else
    {
        type = NRVERTICES;
    }

    while( type< END )
	{
        if(type == NRVERTICES)
        {   
            if(maxVertices == 0)
            {
                fscanf_s( pFile, "NrVertices: %d",&maxVertices);
                bufferVertex        =  new Vertex[maxVertices];                           
            }

            if(fscanf_s( pFile, "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];\n",
                                        &temp_vertices.x, &temp_vertices.y,  &temp_vertices.z,
                                        &temp_normals.x, &temp_normals.y, &temp_normals.z,
                                        &temp_binormals.x, &temp_binormals.y, &temp_binormals.z,
                                        &temp_tgt.x, &temp_tgt.y, &temp_tgt.z,
                                        &temp_uvs.x, &temp_uvs.y) != EOF)
            {
                            bufferVertex[cont].pos    = (temp_vertices);
                            bufferVertex[cont].normal = (temp_normals);
                            bufferVertex[cont].binormal = (temp_binormals);
                            bufferVertex[cont].tgt    = (temp_tgt);
                            bufferVertex[cont].uvs    = (temp_uvs);

                            cont++;

                            if(cont>=maxVertices)
                            {                                
                                type = NRINDICES;                                                                                           

                            }
            }
            else
                type = END;

        }
        else if(type == NRINDICES)
        {
            if(m_noIndices == 0)
            {
                fscanf_s( pFile, "NrIndices: %d",&m_noIndices);
                cont = 0;               
                bufferIndices = new int[m_noIndices];
            }

            if(fscanf_s( pFile, "   %*d.  %f, %f, %f",
                                &temp_indices.x, &temp_indices.y,  &temp_indices.z) != EOF)
            {

                    bufferIndices[cont] = (GLuint)temp_indices.x;
                    cont++;
                    bufferIndices[cont] = (GLuint)temp_indices.y;
                    cont++;
                    bufferIndices[cont] = (GLuint)temp_indices.z;
                    cont++;

                    if(cont>=m_noIndices)
                            {                                
                                cont = 0;
                                type = END;
                            }
            }
            else
                type = END;

        }       
    }   

    glGenBuffers(1, &m_hVertexBuffer);// VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxVertices * sizeof(Vertex), bufferVertex, GL_STATIC_DRAW);

    glGenBuffers(1, &m_hIndexBuffer);// IBO
    glBindBuffer(GL_ARRAY_BUFFER, m_hIndexBuffer);
    glBufferData(GL_ARRAY_BUFFER, (m_noIndices)*sizeof(int), bufferIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    delete[] bufferVertex;
    bufferVertex = NULL;
    delete[] bufferIndices;
    bufferIndices = NULL;
    fclose( pFile );
    pFile = NULL;
    return true;
}
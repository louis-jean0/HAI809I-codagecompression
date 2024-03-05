#include "mesh.h"

///////////////////////////////////////////
////////    CONSTRUCTOR / CLONE    ////////
///////////////////////////////////////////

Mesh::Mesh() : transform_(Eigen::Affine3f::Identity()), canDraw_(false) {}

Mesh Mesh::clone() const
{
    Mesh copy;
    copy.vertices_ = vertices_;
    copy.normals_ = normals_;
    copy.triangles_ = triangles_;
    copy.transform_ = transform_;
    copy.canDraw_ = canDraw_;

    // Local
    copy.vertices_ = this->vertices_;
    copy.normals_ = this->normals_;
    copy.triangles_ = this->triangles_;

    // Global
    copy.transform_ = this->transform_;
    copy.global_vertices_ = this->global_vertices_;
    copy.global_normals_ = this->global_normals_;

    // Draw
    copy.canDraw_ = this->canDraw_;
    copy.meshColor_ = this->meshColor_;

    // 3D Compression Init
    copy.sequence_ = this->sequence_;
    copy.qInfo_ = this->qInfo_;

    return copy;
}

///////////////////////////////////////////
////////     IMPORTS / EXPORTS     ////////
///////////////////////////////////////////

void Mesh::loadFromPLY(const char *filename)
{
    happly::PLYData plyIn(filename);
    plyIn.getVertexPositionsEigen(this->vertices_, this->sequence_);
    plyIn.getFaceIndicesEigen(this->triangles_);
    this->computeNormals();
    this->computeGlobalMeshTransform();
    this->canDraw_ = true;
}

void Mesh::saveToPLY(const char *filename)
{
    happly::PLYData plyOut;
    plyOut.addVertexPositionsEigen(this->vertices_, this->sequence_);
    plyOut.addFaceIndicesEigen(this->triangles_, this->sequence_);
    plyOut.write(filename, happly::DataFormat::Binary);
}

///////////////////////////////////////////
////////           DRAW            ////////
///////////////////////////////////////////

void Mesh::setMeshColor(const Eigen::Vector3f &color)
{
    this->meshColor_ = color;
}

void Mesh::draw(bool displayNormals) const
{
    if (this->canDraw_)
    {
        glColor3fv(meshColor_.data());
        glBegin(GL_TRIANGLES);
        for (const auto &triangle : triangles_)
        {
            for (int j = 0; j < 3; ++j)
            {
                const Eigen::Vector3f &v = global_vertices_[triangle[j]];
                const Eigen::Vector3f &n = global_normals_[triangle[j]];

                glNormal3fv(n.data());
                glVertex3fv(v.data());
            }
        }
        glEnd();

        if (displayNormals)
        {
            glDisable(GL_LIGHTING);
            glColor3f(1.0f, 0.0f, 0.0f);
            glBegin(GL_LINES);
            for (size_t i = 0; i < global_vertices_.size(); ++i)
            {
                const Eigen::Vector3f &v = global_vertices_[i];
                const Eigen::Vector3f &n = global_normals_[i];
                Eigen::Vector3f vn = v + 0.02f * n;

                glVertex3fv(v.data());
                glVertex3fv(vn.data());
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
    }
}

///////////////////////////////////////////
////////      TRANSFORMATIONS      ////////
///////////////////////////////////////////

void Mesh::setPosition(const Eigen::Vector3f &position)
{
    this->transform_.translation() = position;
    this->computeGlobalMeshTransform();
}
void Mesh::setRotation(float angle, const Eigen::Vector3f &axis)
{
    this->transform_.linear() = Eigen::AngleAxisf(angle, axis).toRotationMatrix();
    this->computeGlobalMeshTransform();
}
void Mesh::setScale(const Eigen::Vector3f &scale)
{
    this->transform_.scale(scale);
    this->computeGlobalMeshTransform();
}
void Mesh::setTransform(const Eigen::Affine3f &transform)
{
    this->transform_ = transform;
    this->computeGlobalMeshTransform();
}

void Mesh::translate(const Eigen::Vector3f &translation)
{
    this->transform_.translate(translation);
    this->computeGlobalMeshTransform();
}
void Mesh::rotate(float angle, const Eigen::Vector3f &axis)
{
    this->transform_.rotate(Eigen::AngleAxisf(angle, axis));
    this->computeGlobalMeshTransform();
}

///////////////////////////////////////////
////////           UTILS           ////////
///////////////////////////////////////////

void Mesh::computeNormals()
{
    this->normals_.resize(this->vertices_.size(), Eigen::Vector3f(0.0f, 0.0f, 0.0f));

    for (size_t i = 0; i < triangles_.size(); i++)
    {
        Eigen::Vector3f v0 = this->vertices_[this->triangles_[i][0]];
        Eigen::Vector3f v1 = this->vertices_[this->triangles_[i][1]];
        Eigen::Vector3f v2 = this->vertices_[this->triangles_[i][2]];

        Eigen::Vector3f edge1 = v1 - v0;
        Eigen::Vector3f edge2 = v2 - v0;
        Eigen::Vector3f normal = edge1.cross(edge2).normalized();

        // Accumulate normals for each vertex of the face
        this->normals_[this->triangles_[i][0]] += normal;
        this->normals_[this->triangles_[i][1]] += normal;
        this->normals_[this->triangles_[i][2]] += normal;
    }

    // Normalize all the accumulated normals
    for (size_t i = 0; i < this->normals_.size(); ++i)
    {
        this->normals_[i].normalize();
    }
}

void Mesh::computeGlobalMeshTransform()
{
    size_t sizeV = vertices_.size();
    this->global_vertices_.resize(sizeV);
    this->global_normals_.resize(sizeV);
    for (size_t i = 0; i < sizeV; ++i)
    {
        this->global_vertices_[i] = transform_ * this->vertices_[i];
        this->global_normals_[i] = transform_ * this->normals_[i];
    }
}

///////////////////////////////////////////
////////      3D  COMPRESSION      ////////
///////////////////////////////////////////

Eigen::Vector3f minVector(std::vector<Eigen::Vector3f> &V) { // Créé un vecteur contenant les x, y, z minimaux du vecteur passé en paramètre
    size_t V_size = V.size();
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();

    for(size_t i = 0; i < V_size; i++) {
        if(V[i][0] < minX) {
            minX = V[i][0];
        }
        if(V[i][1] < minY) {
            minY = V[i][1];
        }
        if(V[i][2] < minZ) {
            minZ = V[i][2];
        }
    }
    return Eigen::Vector3f(minX,minY,minZ);
}

Eigen::Vector3f maxVector(const std::vector<Eigen::Vector3f> &V) { // Créé un vecteur contenant les x, y, z maximaux du vecteur passé en paramètre
    size_t V_size = V.size();
    float maxX = -std::numeric_limits<float>::max();
    float maxY = -std::numeric_limits<float>::max();
    float maxZ = -std::numeric_limits<float>::max();

    for(size_t i = 0; i < V_size; i++) {
        if(V[i][0] > maxX) {
            maxX = V[i][0];
        }

        if(V[i][1] > maxY) {
            maxY = V[i][1];
        }

        if(V[i][2] > maxZ) {
            maxZ = V[i][2];
        }
    }
    return Eigen::Vector3f(maxX,maxY,maxZ);
}

void Mesh::init_quantization(int qp)
{
    std::cout << "[MeshCompression3D] - init_quantization : Start..." << std::endl;
    if (qp <= 0 || qp > 30)
    {
        std::cerr << "[MeshCompression3D] - ERROR : qp must be in [1 ; 30]." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Initialize
    this->qInfo_.qp = qp;

    // --- TODO_EX1 --- //
    // PARAMETRES :
    // (std::vector<Eigen::Vector3f>) this->vertices_
    //
    // ACTUALISER :
    // (Eigen::Vector3f) this->qInfo_.minBoundingBox
    // (Eigen::Vector3f) this->qInfo_.maxBoundingBox
    // (double) this->qInfo_.range

    this->qInfo_.minBoundingBox = minVector(this->vertices_);
    this->qInfo_.maxBoundingBox = maxVector(this->vertices_);
    double range_x = this->qInfo_.maxBoundingBox[0] - this->qInfo_.minBoundingBox[0];
    double range_y = this->qInfo_.maxBoundingBox[1] - this->qInfo_.minBoundingBox[1];
    double range_z = this->qInfo_.maxBoundingBox[2] - this->qInfo_.minBoundingBox[2];
    this->qInfo_.range = std::max({range_x,range_y,range_z});
    std::cout<<this->qInfo_.range<<std::endl;
}

void Mesh::encode_geometry_quantization()
{
    std::cout << "[MeshCompression3D] - encode_geometry_quantization : Start..." << std::endl;
    size_t sizeV = this->vertices_.size();
    this->qInfo_.quantizedVertices.resize(sizeV * 3);

    // --- TODO_EX2 --- //
    // PARAMETRES :
    // (std::vector<Eigen::Vector3f>) this->vertices_
    // (int) this->qInfo_.qp
    // (Eigen::Vector3f) this->qInfo_.minBoundingBox
    // (float) this->qInfo_.range
    //
    // ACTUALISER :
    // (std::vector<unsigned int>) this->qInfo_.quantizedVertices [Le vecteur est en une dimension pour XYZ!]

    for(size_t i = 0; i < sizeV; i++) {
        unsigned int quant_x = (this->vertices_[i][0] - this->qInfo_.minBoundingBox[0]) * (pow(2,this->qInfo_.qp)/this->qInfo_.range);
        unsigned int quant_y = (this->vertices_[i][1] - this->qInfo_.minBoundingBox[1]) * (pow(2,this->qInfo_.qp)/this->qInfo_.range);
        unsigned int quant_z = (this->vertices_[i][2] - this->qInfo_.minBoundingBox[2]) * (pow(2,this->qInfo_.qp)/this->qInfo_.range);
        this->qInfo_.quantizedVertices[3*i] = (unsigned int)(quant_x + 0.5);
        this->qInfo_.quantizedVertices[3*i+1] = (unsigned int)(quant_y + 0.5);
        this->qInfo_.quantizedVertices[3*i+2] = (unsigned int)(quant_z + 0.5);
    }
}

void Mesh::decode_geometry_quantization()
{
    std::cout << "[MeshCompression3D] - decode_geometry_quantization : Start..." << std::endl;
    size_t sizeV = this->qInfo_.quantizedVertices.size() / 3.0;

    // --- TODO_EX2 --- //
    // PARAMETRES :
    // (std::vector<unsigned int>) this->qInfo_.quantizedVertices [Le vecteur est en une dimension pour XYZ!]
    // (int) this->qInfo_.qp
    // (Eigen::Vector3f) this->qInfo_.minBoundingBox
    // (float) this->qInfo_.range
    //
    // ACTUALISER :
    // (std::vector<Eigen::Vector3f>) this->vertices_

    for(size_t i = 0; i < sizeV; i++) {
        this->vertices_[i][0] = this->qInfo_.quantizedVertices[3*i] * (this->qInfo_.range / (pow(2,this->qInfo_.qp) - 1)) + this->qInfo_.minBoundingBox[0];
        this->vertices_[i][1] = this->qInfo_.quantizedVertices[3*i+1] * (this->qInfo_.range / (pow(2,this->qInfo_.qp) - 1)) + this->qInfo_.minBoundingBox[1];
        this->vertices_[i][2] = this->qInfo_.quantizedVertices[3*i+2] * (this->qInfo_.range / (pow(2,this->qInfo_.qp) - 1)) + this->qInfo_.minBoundingBox[2];
    }
}

void Mesh::encode_geometry_rANS()
{
    std::cout << "[MeshCompression3D] - encode_geometry_rANS : Start..." << std::endl;

    // --- TODO_EX3 --- //
    // PARAMETRES :
    // (std::vector<unsigned int>) this->qInfo_.quantizedVertices [Le vecteur est en une dimension pour XYZ!]
    //
    // CALCULER :
    // - Taille Séquence (M) : this->qInfo_.quantizedVertices.size()
    // - Fréquence (F)
    // - Fréquence Cumulee (C)
    // ---> Produire un entier naturel non signé dont le type est suffisamment grand pour stocker votre maillage
}

void Mesh::decode_geometry_rANS()
{
    std::cout << "[MeshCompression3D] - decode_geometry_rANS : Start..." << std::endl;

    // --- TODO_EX3 --- //
    // PARAMETRES :
    // Un entier naturel non signé dont le type est suffisamment grand pour stocker votre maillage
    //
    // CALCULER :
    // - Taille Séquence (M)
    // - Fréquence (F)
    // - Fréquence Cumulee (C)
    // - Fonction de fréquence cumulée inverse
    // ---> (std::vector<unsigned int>) this->qInfo_.quantizedVertices [Le vecteur est en une dimension pour XYZ!]
}

void Mesh::encode_mesh(int qp)
{
    this->init_quantization(qp);

    this->encode_geometry_quantization();
    this->encode_geometry_rANS();
}

void Mesh::decode_mesh()
{
    this->decode_geometry_quantization();
    this->decode_geometry_rANS();

    this->computeNormals();
    this->computeGlobalMeshTransform();
    this->canDraw_ = true;
}

///////////////////////////////////////////
////////    DISTORSION MEASURES    ////////
///////////////////////////////////////////

double Mesh::computeSequenceMeshRMSE(const Mesh &meshRef, const Mesh &meshComp)
{
    const std::vector<Eigen::Vector3f> &verticesRef = meshRef.vertices_;
    const std::vector<Eigen::Vector3f> &verticesComp = meshComp.vertices_;

    size_t sizeV = verticesRef.size();
    if (sizeV != verticesComp.size())
    {
        std::cerr << "[RMSE] - ERROR : Meshes have different sizes for vertices or sequence." << std::endl;
        return EXIT_FAILURE;
    }

    // --- TODO_EX1 --- //
    // PARAMETRES :
    // (std::vector<Eigen::Vector3f>) meshRef.vertices_
    // (std::vector<Eigen::Vector3f>) meshComp.vertices_
    // SORTIE :
    // (double) distance RMSE entre meshRef et meshComp

    double mse_x = 0.0;
    double mse_y = 0.0;
    double mse_z = 0.0;
    double MSE = 0.0;
    for(size_t i = 0; i < sizeV; i++) {
        mse_x += pow(meshRef.vertices_[i][0] - meshComp.vertices_[i][0],2);
        mse_y += pow(meshRef.vertices_[i][1] - meshComp.vertices_[i][1],2);
        mse_z += pow(meshRef.vertices_[i][2] - meshComp.vertices_[i][2],2);
    }
    MSE = (1./3.)*((mse_x/sizeV) + (mse_y/sizeV) + (mse_z/sizeV));
    double RMSE = sqrt(MSE);

    return RMSE;
}

double Mesh::distanceEuclidienne(const Eigen::Vector3f A, const Eigen::Vector3f B) {
    return sqrt(pow(B[0] - A[0],2) + pow(B[1] - A[1],2) + pow(B[2] - A[2],2));
}

double Mesh::computeMeshHausdorff(const Mesh &meshRef, const Mesh &meshComp)
{
    const std::vector<Eigen::Vector3f> &verticesRef = meshRef.vertices_;
    const std::vector<Eigen::Vector3f> &verticesComp = meshComp.vertices_;

    size_t sizeRef = verticesRef.size();
    size_t sizeComp = verticesComp.size();

    // --- TODO_EX1 --- //
    // PARAMETRES :
    // (std::vector<Eigen::Vector3f>) meshRef.vertices_
    // (std::vector<Eigen::Vector3f>) meshComp.vertices_
    // SORTIE :
    // (double) distance Hausdorff entre meshRef et meshComp

    std::vector<double> minDistance(sizeRef,std::numeric_limits<double>::max());
    for(size_t i = 0; i < sizeRef; i++) {
        for(size_t j = 0; j < sizeComp; j++) {
            double distance = distanceEuclidienne(meshRef.vertices_[i],meshComp.vertices_[j]);
            if(distance < minDistance[i]) {
                minDistance[i] = distance;
            }
        }
    }

    auto maxMinDistance = std::max_element(std::begin(minDistance),std::end(minDistance));
    double Haussdorff1 = *maxMinDistance;

    minDistance.resize(sizeComp,std::numeric_limits<double>::max());
    for(size_t i = 0; i < sizeComp; i++) {
        for(size_t j = 0; j < sizeRef; j++) {
            double distance = distanceEuclidienne(meshComp.vertices_[i],meshRef.vertices_[j]);
            if(distance < minDistance[i]) {
                minDistance[i] = distance;
            }
        }
    }

    maxMinDistance = std::max_element(std::begin(minDistance),std::end(minDistance));
    double Haussdorff2 = *maxMinDistance;

    return std::max(Haussdorff1,Haussdorff2);
}
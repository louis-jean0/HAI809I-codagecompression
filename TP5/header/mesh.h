#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

#include <GL/glut.h>

#include "Eigen/Dense"
#include "happly.h"

struct QuantizationInfos
{
    int qp = 0;
    float range = 0.0;
    Eigen::Vector3f minBoundingBox = Eigen::Vector3f::Zero();
    Eigen::Vector3f maxBoundingBox = Eigen::Vector3f::Zero();
    std::vector<unsigned int> quantizedVertices;
};

class Mesh
{
private:
    // Local
    std::vector<Eigen::Vector3f> vertices_;
    std::vector<Eigen::Vector3f> normals_;
    std::vector<std::array<unsigned int, 3>> triangles_;

    // Global
    Eigen::Affine3f transform_;
    std::vector<Eigen::Vector3f> global_vertices_;
    std::vector<Eigen::Vector3f> global_normals_;

    // Draw
    bool canDraw_;
    Eigen::Vector3f meshColor_ = Eigen::Vector3f(0.8f, 0.8f, 1.0f);

    // 3D Compression
    std::vector<unsigned int> sequence_;
    QuantizationInfos qInfo_;

    void init_quantization(int qp);
    void encode_geometry_quantization();
    void encode_geometry_rANS();
    void decode_geometry_quantization();
    void decode_geometry_rANS();

    // Utils
    void computeNormals();
    void computeGlobalMeshTransform();

public:
    // Constructor / Clone
    Mesh();
    Mesh clone() const;

    // Imports / Exports
    void loadFromPLY(const char *filename);
    void saveToPLY(const char *filename);

    // Draw
    void setMeshColor(const Eigen::Vector3f &color);
    void draw(bool displayNormals) const;

    // Transformations
    void setPosition(const Eigen::Vector3f &position);
    void setRotation(float angle, const Eigen::Vector3f &axis);
    void setScale(const Eigen::Vector3f &scale);
    void setTransform(const Eigen::Affine3f &transform);
    void translate(const Eigen::Vector3f &translation);
    void rotate(float angle, const Eigen::Vector3f &axis);

    // 3D Compression
    void encode_mesh(int qp);
    void decode_mesh();

    // Distorsion measures
    static double computeSequenceMeshRMSE(const Mesh &meshRef, const Mesh &meshComp);
    static double computeMeshHausdorff(const Mesh &meshRef, const Mesh &meshComp);

    // Distance euclidienne entre deux points du Mesh
    static double distanceEuclidienne(const Eigen::Vector3f A, const Eigen::Vector3f B);
};

#endif

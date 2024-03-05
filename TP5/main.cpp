#include "Eigen/Dense"
#include "handleGlut.h"
#include "mesh.h"

int main(int argc, char **argv)
{
    std::cout << "[C++] : Program Started." << std::endl;

    // --- Args --- //
    if (argc != 3)
    {
        std::cerr << "[USE] : " << argv[0] << " <Mesh.ply> <ParamQP>" << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        // --- Glut init --- //
        glutInit(&argc, argv);

        // --- GLHandler init --- //
        GLHandler &glHandler = GLHandler::getInstance();

        // --- Mesh --- //
        Mesh meshInit;
        meshInit.loadFromPLY(argv[1]);
        meshInit.setMeshColor(Eigen::Vector3f(0.85, 0.85, 1.0));
        meshInit.setPosition(Eigen::Vector3f(-1.0, 1.0, 0.0));

        Mesh meshResult = meshInit.clone();
        meshResult.setMeshColor(Eigen::Vector3f(0.85, 1.0, 0.85));
        meshResult.setPosition(Eigen::Vector3f(1.0, 1.0, 0.0));

        // --- GL Handler Meshes --- //
        glHandler.addMesh(&meshInit);
        glHandler.addMesh(&meshResult);

        // --- 3D Compression --- //
        meshResult.encode_mesh(atoi(argv[2]));
        meshResult.decode_mesh();

        // --- Save --- //
        meshResult.saveToPLY("output.ply");

        // --- Distorsion --- //
        std::cout << "Distance RMSE : " << Mesh::computeSequenceMeshRMSE(meshInit, meshResult) << std::endl;
        std::cout << "Distance Hausdorff : " << Mesh::computeMeshHausdorff(meshInit, meshResult) << std::endl;

        // --- Run --- //
        glHandler.run(1600, 900, "TP5 - Compression3D [HAI819I]");

        return EXIT_SUCCESS;
    }
}
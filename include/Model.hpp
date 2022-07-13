//Store 3D model into glm::vec3 array
//methods:
//calculate volume
//calculate surface area
//translate
//rotate
//scale
//check if 3D point is inside or outside the model assume the model is watertight
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"

class Model{
public:
    Model();
    Model(std::string path);
    ~Model();
    
    void loadModel(std::string& path);
    void translate(glm::vec3& translation);
    void rotate(glm::vec3& axis, float angle);
    void scale(glm::vec3& scale);
    glm::vec3 getVertex(int index);
    glm::vec3 getNormal(int index);
    int getVertexCount();
    int getNormalCount();
    int getIndexCount();
    int getIndex(int index);
    bool isPointInside(glm::vec3& point);
    float getVolume();
    float getSurfaceArea();
    glm::vec3 getVertexIndices(int index);
    int getVertexIndicesSize();
    std::string getExtension();
    void printModel();
    bool isValid();
    std::string getName();
private:
    float _calculateTriangleArea(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3);
    float _calculateTriangleVolume(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3);
    float _calculateSurfaceArea();
    float _calculateVolume();
    int _loadOBJ(std::string& path);
    std::string _name;
    std::string _extension;
    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec3> _normals;
    std::vector<glm::vec3> _uvs;
    std::vector<glm::vec3> _vertexIndices;
    std::vector<glm::vec3> _normalIndices;
    std::vector<glm::vec3> _uvIndices;
    float _volume;
    float _surfaceArea;
    glm::vec3 _min;
    glm::vec3 _max;
    bool _is_min_max_valid;
};
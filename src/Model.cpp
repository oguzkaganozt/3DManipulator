#include "../include/Model.hpp"
#include <string>

Model::Model(){
    this->_volume = 0;
    this->_surfaceArea = 0;
    this->_min = glm::vec3(0,0,0);
    this->_max = glm::vec3(0,0,0);
}

Model::Model(std::string path){
    this->_volume = 0;
    this->_surfaceArea = 0;
    this->_min = glm::vec3(0,0,0);
    this->_max = glm::vec3(0,0,0);

    this->loadModel(path);
}

Model::~Model(){
    this->_vertices.clear();
    this->_normals.clear();
    this->_uvs.clear();
    this->_vertexIndices.clear();
    this->_normalIndices.clear();
    this->_uvIndices.clear();
}

inline float Model::_calculateTriangleArea(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3){
	// float ax = p2.x - p1.x;
	// float ay = p2.y - p1.y;
	// float az = p2.z - p1.z;
	// float bx = p3.x - p1.x;
	// float by = p3.y - p1.y;
	// float bz = p3.z - p1.z;
    auto tmp_p = p2 - p1;
    auto tmp_q = p3 - p1;    

	// float cx = ay * bz - az * by;
	// float cy = az * bx - ax * bz;
	// float cz = ax * by - ay * bx;
    
    auto tmp_c = glm::cross(tmp_q,tmp_p);

	// return (float)0.5 * sqrt(cx*cx + cy * cy + cz * cz);
    return glm::length(tmp_c) * 0.5;
}

inline float Model::_calculateTriangleVolume(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3){
    auto v1 = p2 - p1;
    auto v2 = p3 - p1;
    auto v3 = glm::cross(v1, v2);
    return glm::dot(v3, p1)/6.0f;
}

float Model::_calculateSurfaceArea(){
    double area = 0;
    for(auto& v : _vertexIndices){
        glm::vec3 p1 = this->_vertices[v.x];
        glm::vec3 p2 = this->_vertices[v.y];
        glm::vec3 p3 = this->_vertices[v.z];
        area += this->_calculateTriangleArea(p1, p2, p3);
    }

    _surfaceArea = (float)area;
    return (float)area;
}

float Model::_calculateVolume(){
    double volume = 0;
    for(auto& v : _vertexIndices){
        glm::vec3 p1 = this->_vertices[v.x];
        glm::vec3 p2 = this->_vertices[v.y];
        glm::vec3 p3 = this->_vertices[v.z];
        volume += this->_calculateTriangleVolume(p1, p2, p3);
    }

    _volume = (float)volume;
    return (float)volume;
}

void Model::loadModel(std::string& path){
    std::string extension = path.substr(path.find_last_of(".") + 1);
    _extension = extension;
    _name = path.substr(path.find_last_of("/") + 1);
    if(extension == "obj"){
        try{
            this->_loadOBJ(path);
        }
        catch(std::exception& e){
            throw std::runtime_error("Error loading OBJ file: " + std::string(e.what()));
        }
        this->_calculateSurfaceArea();
        this->_calculateVolume();
    }
    else{
        throw std::invalid_argument("File extension not supported");
    }
}

#define FACE_VERTICES 3
inline int Model::_loadOBJ(std::string& path){
    std::ifstream file(path);
    std::string line;
    std::string type;
    std::string value[FACE_VERTICES];
    std::stringstream ss;
    if(file.is_open()){
        while(getline(file, line)){
            type.clear();
            ss.clear();
            ss.str(line);
            ss >> type;
            if(type == "v"){    //vertex
                ss >> value[0] >> value[1] >> value[2];
                _vertices.push_back(glm::vec3(std::stof(value[0]), std::stof(value[1]), std::stof(value[2])));
            }
            else if(type == "vn"){  //normal
                ss >> value[0] >> value[1] >> value[2];
                _normals.push_back(glm::vec3(std::stof(value[0]), std::stof(value[1]), std::stof(value[2])));
            }
            else if(type == "vt"){  //texture vertex
                ss >> value[0] >> value[1] >> value[2];
                _uvs.push_back(glm::vec3(std::stof(value[0]), std::stof(value[1]), std::stof(value[2])));
            }
            else if(type == "f"){   //face
                if(_uvs.size() == 0){
                    if(_normals.size() == 0){
                        ss >> value[0] >> value[1] >> value[2];
                        _vertexIndices.push_back(glm::vec3(std::stoi(value[0]) - 1, std::stoi(value[1]) - 1, std::stoi(value[2]) - 1));
                    }
                    else{
                        int vertices[FACE_VERTICES];
                        int normals[FACE_VERTICES];
                        ss >> value[0] >> value[1] >> value[2];

                        for (int i = 0; i < FACE_VERTICES; i++)
                        {
                            if(value[i] == "")
                                throw std::runtime_error("Error loading OBJ file: face vertex index missing");

                            vertices[i] = std::stoi(value[i].substr(0, value[i].find("//")));
                            normals[i] = std::stoi(value[i].substr(value[i].find("//") + 2));
                        }

                        _vertexIndices.push_back(glm::vec3(vertices[0] - 1, vertices[1] - 1, vertices[2] - 1));
                        _normalIndices.push_back(glm::vec3(normals[0] - 1, normals[1] - 1, normals[2] - 1));
                    }
                }
                else if(_normals.size() == 0){
                    int vertices[FACE_VERTICES];
                    int uvs[FACE_VERTICES];
                    ss >> value[0] >> value[1] >> value[2];

                    for (int i = 0; i < FACE_VERTICES; i++)
                    {
                        if(value[i] == "")
                            throw std::runtime_error("Error loading OBJ file: face vertex index missing");

                        vertices[i] = std::stoi(value[i].substr(0, value[i].find("/")));
                        uvs[i] = std::stoi(value[i].substr(value[i].find("/") + 1));
                    }

                    _vertexIndices.push_back(glm::vec3(vertices[0] - 1, vertices[1] - 1, vertices[2] - 1));
                    _uvIndices.push_back(glm::vec3(uvs[0] - 1, uvs[1] - 1, uvs[2] - 1));
                }
                else{
                    int vertices[FACE_VERTICES];
                    int normals[FACE_VERTICES];
                    int uvs[FACE_VERTICES];
                    ss >> value[0] >> value[1] >> value[2];

                    for (int i = 0; i < FACE_VERTICES; i++)
                    {
                        if(value[i] == "")
                            throw std::runtime_error("Error loading OBJ file: face vertex index missing");

                        vertices[i] = std::stoi(value[i].substr(0, value[i].find("/")));
                        uvs[i] = std::stoi(value[i].substr(value[i].find("/") + 1, value[i].find_last_of("/")));
                        normals[i] = std::stoi(value[i].substr(value[i].find_last_of("/") + 1));
                    }

                    _vertexIndices.push_back(glm::vec3(vertices[0] - 1, vertices[1] - 1, vertices[2] - 1));
                    _uvIndices.push_back(glm::vec3(uvs[0] - 1, uvs[1] - 1, uvs[2] - 1));
                    _normalIndices.push_back(glm::vec3(normals[0] - 1, normals[1] - 1, normals[2] - 1));
                }
            }
        }
        file.close();
    }
    else{
        throw std::runtime_error("Error opening OBJ file");
    }
    return 0;
}

void Model::translate(glm::vec3& translation){
    for(auto& vertex : _vertices)
        vertex += translation;
    _is_min_max_valid = false;
}

void Model::scale(glm::vec3& scale){
    for(auto& vertex : _vertices){
        vertex *= scale;
    }
    _is_min_max_valid = false;
}

void Model::rotate(glm::vec3& axis, float angle){
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
    for(auto& vertex : _vertices){
        vertex = glm::vec3(rotation * glm::vec4(vertex, 1.0f));
    }
    _is_min_max_valid = false;
}

bool Model::isValid(){
    return _vertices.size() > 0;
}

glm::vec3 Model::getVertexIndices(int index){
    return _vertexIndices[index];
}

int Model::getVertexIndicesSize(){
    return _vertexIndices.size();
}

glm::vec3 Model::getVertex(int index){
    return _vertices[index];
}

glm::vec3 Model::getNormal(int index){
    return _normals[index];
}

int Model::getVertexCount(){
    return _vertices.size();
}

int Model::getNormalCount(){
    return _normals.size();
}

bool Model::isPointInside(glm::vec3& point){

    if(_is_min_max_valid == false){
        auto min = _vertices[0];
        auto max = _vertices[0];

        for(auto& vertex : _vertices){
            if(vertex.x < min.x)
                min.x = vertex.x;
            if(vertex.y < min.y)
                min.y = vertex.y;
            if(vertex.z < min.z)
                min.z = vertex.z;

            if(vertex.x > max.x)
                max.x = vertex.x;
            if(vertex.y > max.y)
                max.y = vertex.y;
            if(vertex.z > max.z)
                max.z = vertex.z;
        }
        _min = min;
        _max = max;
        _is_min_max_valid = true;
    }
    
    if(point.x < _min.x || point.x > _max.x || point.y < _min.y || point.y > _max.y || point.z < _min.z || point.z > _max.z)
        return false;

    return true;
}

float Model::getVolume(){
    return _volume;
}

float Model::getSurfaceArea(){
    return _surfaceArea;
}

std::string Model::getName(){
    return _name;
}

std::string Model::getExtension(){
    return _extension;
}

void Model::printModel(){

    std::ofstream file("model.dump", std::ios::out | std::ios::trunc);

    file << "MODEL: " << _name << std::endl;
    file << "Vertices: " << _vertices.size() << std::endl;
    file << "Normals: " << _normals.size() << std::endl;
    file << "UVs: " << _uvs.size() << std::endl;
    file << "VertexIndices: " << _vertexIndices.size() << std::endl;
    file << "NormalIndices: " << _normalIndices.size() << std::endl;
    file << "UVIndices: " << _uvIndices.size() << std::endl;
    file << "----------------------------------------" << std::endl;

    file << "Vertices: " << std::endl;
    for(auto& vertex : _vertices){
        file << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }
    file << "----------------------------------------" << std::endl;

    file << "Normals: " << std::endl;
    for(auto& normal : _normals){
        file << normal.x << " " << normal.y << " " << normal.z << std::endl;
    }
    file << "----------------------------------------" << std::endl;

    file << "UVs: " << std::endl;
    for(auto& uv : _uvs){
        file << uv.x << " " << uv.y << std::endl;
    }
    file << "----------------------------------------" << std::endl;

    file << "Vertex Indices: " << std::endl;
    for(auto& vertexIndex : _vertexIndices){
        file << vertexIndex.x << " " << vertexIndex.y << " " << vertexIndex.z << std::endl;
    }
    file << "----------------------------------------" << std::endl;

    file << "Normal Indices: " << std::endl;
    for(auto& normalIndex : _normalIndices){
        file << normalIndex.x << " " << normalIndex.y << " " << normalIndex.z << std::endl;
    }
    file << "----------------------------------------" << std::endl;

    file << "UV Indices: " << std::endl;
    for(auto& uvIndex : _uvIndices){
        file << uvIndex.x << " " << uvIndex.y << " " << uvIndex.z << std::endl;
    }
    file << "----------------------------------------" << std::endl;

    file.close();
}


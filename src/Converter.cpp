#include "../include/Converter.hpp"
#include "../include/Model.hpp"
#include <string>
#include <exception>
#include <vector>
#include <iostream>
#include <fstream>

bool Converter::convert(Model& model, std::string path){
    if(model.isValid()){
        _model = model;
        auto extension = path.substr(path.find_last_of(".") + 1);

        if(extension.compare("stl") == 0){
            return convertToBinarySTL(model, path);
        }
        else
            return false;

        return true;
    }
    else
        return false;
}

bool Converter::convertToBinarySTL(Model& model, std::string path)
{
    std::ofstream file(path, std::ios::out | std::ios::binary);

    //Header
    int size = model.getVertexIndicesSize();
    const char header[80] = "HEADER";
    file.write(header, 80);
    file.write((char*)&size, sizeof(int));

    for (int i = 0; i < model.getVertexIndicesSize(); i++)
    {
        auto p1 = model.getVertex((unsigned int)model.getVertexIndices(i).x);
        auto p2 = model.getVertex((unsigned int)model.getVertexIndices(i).y);
        auto p3 = model.getVertex((unsigned int)model.getVertexIndices(i).z);

        auto vec1 = p1 - p2;
        auto vec2 = p1 - p3;

        auto normal = glm::cross(vec1, vec2);
        normal = glm::normalize(normal);

        // Normal
        file.write((char*)&normal.x, sizeof(float));
        file.write((char*)&normal.y, sizeof(float));
        file.write((char*)&normal.z, sizeof(float));

        // Points
        file.write((char*)&p1.x, sizeof(float));
        file.write((char*)&p1.y, sizeof(float));
        file.write((char*)&p1.z, sizeof(float));

        file.write((char*)&p2.x, sizeof(float));
        file.write((char*)&p2.y, sizeof(float));
        file.write((char*)&p2.z, sizeof(float));

        file.write((char*)&p3.x, sizeof(float));
        file.write((char*)&p3.y, sizeof(float));
        file.write((char*)&p3.z, sizeof(float));

        //Attribute byte count
        uint16_t attr = 0;
        file.write((char*)&attr, sizeof(uint16_t));
    }

    return true;
}
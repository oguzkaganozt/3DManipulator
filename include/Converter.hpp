//Convert the 3D model into other file formats
//Only implement STL file format
#pragma once
#include "../include/Model.hpp"

class Converter{
public:
    bool convert(Model& model, std::string path);
    bool convertToBinarySTL(Model& model, std::string path);
private:
    Model _model;
};
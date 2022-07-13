#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/Model.hpp"
#include "../include/Converter.hpp"
#include "../include/FastConverter.hpp"

int main(int argc, char** argv)
{
    std::string path;
    int choice;
    bool exit = false;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "Hello World!" << std::endl;
    std::cout<<"Fast Converter"<<std::endl;

    if(argc < 3)
    {
        std::cout<<"Enter the path of the model:"<<std::endl;
        std::cin>>path;
    }
    else{
        path = argv[2];
        std::cout << "Path: " << path << std::endl;
    }        

    Model model(path);
    std::cout<<"Model loaded"<<std::endl;
    
    std::cout << "Vertex count of the model is: " << model.getVertexCount() << std::endl;
    std::cout << "Surface area of the model is: " << model.getSurfaceArea() << std::endl;
    std::cout << "Volume of the model is: " << model.getVolume() << std::endl;

    //ask for transformation
    while (!exit)
    {
        std::cout << "------------------------------------" << std::endl;
        std::cout << "Choose the operation you want to perform" << std::endl;
        std::cout<<"1. Translate"<<std::endl;
        std::cout<<"2. Scale"<<std::endl;
        std::cout<<"3. Rotate"<<std::endl;
        std::cout<<"4. Is point inside the mesh"<<std::endl;
        std::cout<<"5. Convert file"<<std::endl;
        std::cout<<"6. Exit"<<std::endl;
        std::cin>>choice;

        switch(choice)
        {
            case 1:
            {
                std::cout<<"Enter the translation vector x y z with spaces"<<std::endl;
                glm::vec3 translation;
                std::cin>>translation.x>>translation.y>>translation.z;
                model.translate(translation);
                std::cout<<"Model translated"<<std::endl;
                break;
            }
            case 2:
            {
                std::cout<<"Enter the scale vector x y z with spaces"<<std::endl;
                glm::vec3 scale;
                std::cin>>scale.x>>scale.y>>scale.z;
                model.scale(scale);
                std::cout<<"Model scaled"<<std::endl;
                break;
            }
            case 3:
            {
                std::cout<<"Enter the axis vector x y z with spaces"<<std::endl;
                glm::vec3 axis;
                std::cin>>axis.x>>axis.y>>axis.z;
                std::cout<<"Enter the angle"<<std::endl;
                float angle;
                std::cin>>angle;
                model.rotate(axis,angle);
                std::cout<<"Model rotated"<<std::endl;
                break;
            }
            case 4:
            {
                std::cout<<"Enter the point for inside check x y z with spaces"<<std::endl;
                glm::vec3 point;
                std::cin>>point.x>>point.y>>point.z;
                if(model.isPointInside(point))
                    std::cout<<"Point is inside the model"<<std::endl;
                else
                    std::cout<<"Point is outside the model"<<std::endl;
                break;
            }
            case 5:
            {
                std::cout<<"Enter export file name with extension"<<std::endl;
                std::string path;
                std::cin>>path;
                Converter converter;
                if(converter.convert(model, path))
                    std::cout<<"File converted"<<std::endl;
                else
                    std::cout<<"Output file could not be created or extension not supported"<<std::endl;
                break;
            }
            case 6:
            {
                std::cout<<"Exiting"<<std::endl;
                exit = true;
                break;
            }
            default:
            {
                std::cout<<"Invalid choice"<<std::endl;
                break;
            }
        }
        
        //Print the debug info
        model.printModel();
    }
    
    return 0;
}
//
//  JSONPacker.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "JSONPacker.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace cocos2d;

namespace JSONPacker {
    TetrominoState unpackTetrominoJSON(std::string json, TetrominoType type)
    {
        rapidjson::Document document;
        document.Parse<0>(json.c_str());
        
        const char* typeStr;
        switch (type) {
            case TetrominoType::T: typeStr = "T"; break;
            case TetrominoType::I: typeStr = "I"; break;
            case TetrominoType::J: typeStr = "J"; break;
            case TetrominoType::S: typeStr = "S"; break;
            case TetrominoType::Z: typeStr = "Z"; break;
            case TetrominoType::O: typeStr = "O"; break;
            case TetrominoType::L: typeStr = "L"; break;
        }
        
        TetrominoState rtnValue;

        // Get color
        rapidjson::Value& tetrominoDoc = document[typeStr];
        rapidjson::Value& colorDoc = tetrominoDoc["color"];
        int r = colorDoc["r"].GetInt();
        int g = colorDoc["g"].GetInt();
        int b = colorDoc["b"].GetInt();
        Color3B color = Color3B(r, g, b);

        // Get rotations
        std::vector<std::vector<Coordinate>> rotations;
        rapidjson::Value& rotationsDoc = tetrominoDoc["rotations"];
        for (int i = 0; i < rotationsDoc.Size(); i++) {
            rapidjson::Value& rotaionDoc = rotationsDoc[i];
            std::vector<Coordinate> rotation;
            for (int j = 0; j < rotaionDoc.Size(); j++) {
                rapidjson::Value& dot = rotaionDoc[j];
                int x = dot["x"].GetInt();
                int y = dot["y"].GetInt();
                rotation.push_back(Coordinate(x, y));
            }
            rotations.push_back(rotation);
        }

        // Make tetrominoState
        TetrominoState state;
        state.color = color;
        state.rotations = rotations;

        return state;
    }
    
    std::string packTetrominoJSON(TetrominoState tetrominoState)
    {
    }
}



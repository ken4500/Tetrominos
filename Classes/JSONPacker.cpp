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
    GameState unpackGameStateJSON(std::string json)
    {
        rapidjson::Document document;
        document.Parse<0>(json.c_str());
        
        GameState gameState;
        
        gameState.score = document["score"].GetInt();
        gameState.name = document["name"].GetString();
        gameState.gameOver = document["gameOver"].GetBool();
        
        rapidjson::Value& columns = document["board"];
        for (rapidjson::SizeType column = 0; column < columns.Size(); ++column) {
            rapidjson::Value& blocksJson = columns[column];
            
            std::vector<Color3B> blocks;
            for (rapidjson::SizeType index = 0; index < blocksJson.Size(); index++) {
                rapidjson::Value& block = blocksJson[index];
                int r = block["r"].GetInt();
                int g = block["g"].GetInt();
                int b = block["b"].GetInt();
                Color3B color = Color3B(r, g, b);
                blocks.push_back(color);
            }
            gameState.board.push_back(blocks);
        }
        return gameState;
    }

    std::string packGameState(const GameState data)
    {
        rapidjson::Document document;
        document.SetObject();
        document.AddMember("score", data.score, document.GetAllocator());
        document.AddMember("name", data.name.c_str(), document.GetAllocator());
        document.AddMember("gameOver", data.gameOver, document.GetAllocator());

        rapidjson::Value colomun(rapidjson::kArrayType);
        for (int col = 0; col < data.board.size(); col++) {
            rapidjson::Value blocks(rapidjson::kArrayType);
            for (int index = 0; index < data.board[col].size();index++) {
                Color3B color = data.board[col][index];
                rapidjson::Value colorJson(rapidjson::kObjectType);
                colorJson.AddMember("r", color.r, document.GetAllocator());
                colorJson.AddMember("g", color.g, document.GetAllocator());
                colorJson.AddMember("b", color.b, document.GetAllocator());
                blocks.PushBack(colorJson, document.GetAllocator());
            }
            colomun.PushBack(blocks, document.GetAllocator());
        }

        document.AddMember("board", colomun, document.GetAllocator());
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        
        std::string returnString(buffer.GetString(), buffer.Size());
        
        return returnString;
    }

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
}



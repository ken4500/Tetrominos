//
//  TetrominoBag.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/02.
//
//

#include "TetrominoBag.h"

TetrominoBag::TetrominoBag() :
bag(std::vector<TetrominoType>()),
randomGenerator(std::default_random_engine((unsigned int)time(0)))
{
}

TetrominoType TetrominoBag::getTetromino()
{
    if (this->bag.empty()) {
        this->bag.push_back(TetrominoType::I);
        this->bag.push_back(TetrominoType::J);
        this->bag.push_back(TetrominoType::L);
        this->bag.push_back(TetrominoType::O);
        this->bag.push_back(TetrominoType::S);
        this->bag.push_back(TetrominoType::T);
        this->bag.push_back(TetrominoType::Z);
    }
    
    std::shuffle(bag.begin(), bag.end(), this->randomGenerator);
    TetrominoType rtnValue = this->bag.back();
    this->bag.pop_back();
    return rtnValue;
}
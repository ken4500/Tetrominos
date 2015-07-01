//
//  Grid.h
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#ifndef __Tetrominos__Grid__
#define __Tetrominos__Grid__

#include "cocos2d.h"
#include "Coordinate.h"

class Grid : public cocos2d::Sprite {
public:
    CREATE_FUNC(Grid);

protected:
    bool init() override;

};

#endif /* defined(__Tetrominos__Grid__) */

//
//  PreviewGrid.h
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/10.
//
//

#ifndef __Tetrominos__PreviewGrid__
#define __Tetrominos__PreviewGrid__

#include "Cocos2d.h"
#include "JSONPacker.h"

class PreviewGrid : public cocos2d::Sprite
{
public:
    CREATE_FUNC(PreviewGrid);
    void setState(JSONPacker::GameState state);

private:
    bool init() override;
    
};

#endif /* defined(__Tetrominos__PreviewGrid__) */

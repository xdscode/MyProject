//
//  MeshSprite.cpp
//  EmpireLegion3.0
//
//  Created by gamesoul-empirelegion on 15-2-10.
//
//

#include "MeshSprite.h"


#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif


MeshSprite* MeshSprite::create()
{
    MeshSprite *sprite = new (std::nothrow) MeshSprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool MeshSprite::initWithSpriteFrameName(const std::string& spriteFrameName)
{
    CCASSERT(spriteFrameName.size() > 0, "Invalid spriteFrameName");
    
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    return initWithSpriteFrame(frame);
}
void MeshSprite::setMeshPoints(Vec2 p1,Vec2 p2,Vec2 p3,Vec2 p4)
{
//    x1=xx1;
//    x2=xx2;
//    x3=xx3;
//    x4=xx4;
//    y1=yy1;
//    y2=yy2;
//    y3=yy3;
//    y4=yy4;
    //    Vec2 a2=convertToWorldSpace(Vec2(x1,y1));
    //    Vec2 c2=convertToWorldSpace(Vec2(x2,y2));
    //    Vec2 b2=convertToWorldSpace(Vec2(x3,y3));
    //    Vec2 d2=convertToWorldSpace(Vec2(x4,y4));
    
//    Vec2 c2=convertToWorldSpace(Vec2(x1,y1));
//    Vec2 a2=convertToWorldSpace(Vec2(x2,y2));
//    Vec2 d2=convertToWorldSpace(Vec2(x3,y3));
//    Vec2 b2=convertToWorldSpace(Vec2(x4,y4));
    
    Vec2 c2=(p1);
    Vec2 a2=(p2);
    Vec2 d2=(p3);
    Vec2 b2=(p4);
    
    
    _quad.bl.vertices = Vec3( RENDER_IN_SUBPIXEL(a2.x), RENDER_IN_SUBPIXEL(a2.y), _positionZ );
    _quad.br.vertices = Vec3( RENDER_IN_SUBPIXEL(b2.x), RENDER_IN_SUBPIXEL(b2.y), _positionZ );
    _quad.tl.vertices = Vec3( RENDER_IN_SUBPIXEL(c2.x), RENDER_IN_SUBPIXEL(c2.y), _positionZ );
    _quad.tr.vertices = Vec3( RENDER_IN_SUBPIXEL(d2.x), RENDER_IN_SUBPIXEL(d2.y), _positionZ );
    
//    CCLOG("after:(%f,%f),(%f,%f),(%f,%f),(%f,%f)",_quad.bl.vertices.x,_quad.bl.vertices.y,_quad.br.vertices.x,_quad.br.vertices.y,_quad.tl.vertices.x,_quad.tl.vertices.y,_quad.tr.vertices.x,_quad.tr.vertices.y);
    
}

void MeshSprite::setTextureRect(const Rect& rect, bool rotated, const Size& untrimmedSize)
{
    _rectRotated = rotated;
    
    setContentSize(untrimmedSize);
    setVertexRect(rect);
    setTextureCoords(rect);
    
    Vec2 relativeOffset = _unflippedOffsetPositionFromCenter;
    
    // issue #732
    if (_flippedX)
    {
        relativeOffset.x = -relativeOffset.x;
    }
    if (_flippedY)
    {
        relativeOffset.y = -relativeOffset.y;
    }
    
    _offsetPosition.x = relativeOffset.x + (_contentSize.width - _rect.size.width) / 2;
    _offsetPosition.y = relativeOffset.y + (_contentSize.height - _rect.size.height) / 2;
    
    // rendering using batch node
    if (_batchNode)
    {
        // update dirty_, don't update recursiveDirty_
        setDirty(true);
    }
    else
    {
        // self rendering
        
        // Atlas: Vertex
        float x1 = 0 + _offsetPosition.x;
        float y1 = 0 + _offsetPosition.y;
        float x2 = x1 + _rect.size.width;
        float y2 = y1 + _rect.size.height;
        
        // Don't update Z.
//        _quad.bl.vertices = Vec3(x1, y1, 0);
//        _quad.br.vertices = Vec3(x2, y1, 0);
//        _quad.tl.vertices = Vec3(x1, y2, 0);
//        _quad.tr.vertices = Vec3(x2, y2, 0);
    }
}



void MeshSprite::updateTransform(void)
{
    //    CCASSERT(_batchNode, "updateTransform is only valid when Sprite is being rendered using an SpriteBatchNode");
    //
    //    // recalculate matrix only if it is dirty
    //    if( isDirty() ) {
    //
    //        // If it is not visible, or one of its ancestors is not visible, then do nothing:
    //        if( !_visible || ( _parent && _parent != _batchNode && static_cast<Sprite*>(_parent)->_shouldBeHidden) )
    //        {
    //            _quad.br.vertices = _quad.tl.vertices = _quad.tr.vertices = _quad.bl.vertices = Vec3(0,0,0);
    //            _shouldBeHidden = true;
    //        }
    //        else
    //        {
    //            _shouldBeHidden = false;
    //
    //            if( ! _parent || _parent == _batchNode )
    //            {
    //                _transformToBatch = getNodeToParentTransform();
    //            }
    //            else
    //            {
    //                CCASSERT( dynamic_cast<Sprite*>(_parent), "Logic error in Sprite. Parent must be a Sprite");
    //                const Mat4 &nodeToParent = getNodeToParentTransform();
    //                Mat4 &parentTransform = static_cast<Sprite*>(_parent)->_transformToBatch;
    //                _transformToBatch = parentTransform * nodeToParent;
    //            }
    //
    //            //
    //            // calculate the Quad based on the Affine Matrix
    //            //
    //
    //            Size &size = _rect.size;
    //
    //            float x1 = _offsetPosition.x;
    //            float y1 = _offsetPosition.y;
    //
    //            float x2 = x1 + size.width;
    //            float y2 = y1 + size.height;
    //            float x = _transformToBatch.m[12];
    //            float y = _transformToBatch.m[13];
    //
    //            float cr = _transformToBatch.m[0];
    //            float sr = _transformToBatch.m[1];
    //            float cr2 = _transformToBatch.m[5];
    //            float sr2 = -_transformToBatch.m[4];
    //            float ax = x1 * cr - y1 * sr2 + x;
    //            float ay = x1 * sr + y1 * cr2 + y;
    //
    //            float bx = x2 * cr - y1 * sr2 + x;
    //            float by = x2 * sr + y1 * cr2 + y;
    //
    //            float cx = x2 * cr - y2 * sr2 + x;
    //            float cy = x2 * sr + y2 * cr2 + y;
    //
    //            float dx = x1 * cr - y2 * sr2 + x;
    //            float dy = x1 * sr + y2 * cr2 + y;
    //
    //            _quad.bl.vertices = Vec3( RENDER_IN_SUBPIXEL(ax), RENDER_IN_SUBPIXEL(ay), _positionZ );
    //            _quad.br.vertices = Vec3( RENDER_IN_SUBPIXEL(bx), RENDER_IN_SUBPIXEL(by), _positionZ );
    //            _quad.tl.vertices = Vec3( RENDER_IN_SUBPIXEL(dx), RENDER_IN_SUBPIXEL(dy), _positionZ );
    //            _quad.tr.vertices = Vec3( RENDER_IN_SUBPIXEL(cx), RENDER_IN_SUBPIXEL(cy), _positionZ );
    //        }
    //
    //        // MARMALADE CHANGE: ADDED CHECK FOR nullptr, TO PERMIT SPRITES WITH NO BATCH NODE / TEXTURE ATLAS
    //        if (_textureAtlas)
    //        {
    //            _textureAtlas->updateQuad(&_quad, _atlasIndex);
    //        }
    //
    //        _recursiveDirty = false;
    //        setDirty(false);
    //    }
    
    Vec2 c2=convertToWorldSpace(Vec2(x1,y1));
    Vec2 a2=convertToWorldSpace(Vec2(x2,y2));
    Vec2 d2=convertToWorldSpace(Vec2(x3,y3));
    Vec2 b2=convertToWorldSpace(Vec2(x4,y4));
    
    _quad.bl.vertices = Vec3( RENDER_IN_SUBPIXEL(a2.x), RENDER_IN_SUBPIXEL(a2.y), _positionZ );
    _quad.br.vertices = Vec3( RENDER_IN_SUBPIXEL(b2.x), RENDER_IN_SUBPIXEL(b2.y), _positionZ );
    _quad.tl.vertices = Vec3( RENDER_IN_SUBPIXEL(c2.x), RENDER_IN_SUBPIXEL(c2.y), _positionZ );
    _quad.tr.vertices = Vec3( RENDER_IN_SUBPIXEL(d2.x), RENDER_IN_SUBPIXEL(d2.y), _positionZ );
    
    // MARMALADE CHANGED
    // recursively iterate over children
    /*    if( _hasChildren )
     {
     // MARMALADE: CHANGED TO USE Node*
     // NOTE THAT WE HAVE ALSO DEFINED virtual Node::updateTransform()
     arrayMakeObjectsPerformSelector(_children, updateTransform, Sprite*);
     }*/
    Node::updateTransform();
}




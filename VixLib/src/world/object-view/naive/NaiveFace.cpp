#include "pch.h"
#include "include\world\object-view\naive\NaiveFace.h"


namespace VixLib {
namespace world {

NaiveFace::NaiveFace(int vert_offset, 
                     int index_offset) :
    vert_offset(vert_offset), 
    index_offset(index_offset) {
}


NaiveFaceKey::NaiveFaceKey(Position pos, Direction dir) :
    pos(pos), dir(dir) { 
}


}
}
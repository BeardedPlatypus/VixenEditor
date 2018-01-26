#include "pch.h"
#include "include\world\selection\VoxelFace.h"


namespace VixLib {
namespace world {

VoxelFace::VoxelFace(Position pos, 
                     Direction dir,
                     const Node& node) :
    m_pos(pos),
    m_dir(dir),
    m_node(node) {
}

}
}
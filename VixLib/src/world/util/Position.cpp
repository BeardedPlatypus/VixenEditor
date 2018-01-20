#include "pch.h"
#include "include\world\util\Position.h"


namespace VixLib {
namespace world {


Position::Position(int x, int y, int z) : x(x), y(y), z(z) {
}


Position::Position(DirectX::XMINT3 pos) : Position(pos.x, pos.y, pos.z) {
}


Position::Position() : Position(0, 0, 0) {
}



}
}
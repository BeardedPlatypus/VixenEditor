#include "pch.h"
#include "include\world\object-view\naive\NaiveObjectView.h"


namespace VixLib {
namespace world {

// ----------------------------------------------------------------------------
// Constructor | Destructor
// ----------------------------------------------------------------------------
NaiveObjectView::NaiveObjectView() :
  vertices(std::vector<graphics::VertexPositionNormalColor>()),
  indices(std::vector<unsigned short>()),
  face_map(std::unordered_map<NaiveFaceKey, NaiveFace*>()),
  faces(std::vector<NaiveFace*>()) {
}


// ----------------------------------------------------------------------------
NaiveObjectView::~NaiveObjectView() {
  for (NaiveFace* p_face : this->faces) {
    delete p_face;
  }
}


// ----------------------------------------------------------------------------
// Interaction Methods
// ----------------------------------------------------------------------------
void NaiveObjectView::addFace(Position pos, Direction dir,
                              DirectX::XMFLOAT3 col) {
  NaiveFaceKey key = NaiveFaceKey(pos, dir);
  try {
    // There already exists a face at the specified position
    // Only the colour of the vertices needs to be updated
    NaiveFace* face = this->face_map.at(key);

    this->vertices[face->vert_offset + 0].color = col;
    this->vertices[face->vert_offset + 1].color = col;
    this->vertices[face->vert_offset + 2].color = col;
    this->vertices[face->vert_offset + 3].color = col;
  }
  catch (const std::out_of_range& e) {
    // No face exists yet and needs to be added to the specified position
    int cur_offset_vertices = this->vertices.size();
    int cur_offset_indices = this->indices.size();

    // Determine vertices
    graphics::VertexPositionNormalColor v0, v1, v2, v3;
    float x = static_cast<float>(pos.x);
    float y = static_cast<float>(pos.y);
    float z = static_cast<float>(pos.z);

    DirectX::XMFLOAT3 normal;
    switch (dir) {
      // -----------------------------------------------
    case Direction::XNEG:
      v0 = { DirectX::XMFLOAT3(x - 0.5f, 
                               y - 0.5f, 
                               z + 0.5f),
             DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f),
             col };
      v1 = { DirectX::XMFLOAT3(x - 0.5,
                               y - 0.5,
                               z - 0.5),
             DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f),
             col };
      v2 = { DirectX::XMFLOAT3(x - 0.5,
                               y + 0.5,
                               z - 0.5),
             DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f),
             col };
      v3 = { DirectX::XMFLOAT3(x - 0.5,
                               y + 0.5,
                               z + 0.5),
             DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f),
             col };
      break;
      // -----------------------------------------------
    case Direction::XPOS:
      v0 = { DirectX::XMFLOAT3(x + 0.5f,
                               y - 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(+1.0f,  0.0f,  0.0f),
             col };
      v1 = { DirectX::XMFLOAT3(x + 0.5f,
                               y - 0.5f,
                               z + 0.5f),
             DirectX::XMFLOAT3(+1.0f,  0.0f,  0.0f),
             col };
      v2 = { DirectX::XMFLOAT3(x + 0.5f,
                               y + 0.5f,
                               z + 0.5f),
             DirectX::XMFLOAT3(+1.0f,  0.0f,  0.0f),
             col };
      v3 = { DirectX::XMFLOAT3(x + 0.5f,
                               y + 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(+1.0f,  0.0f,  0.0f),
             col };
      break;
      // -----------------------------------------------
    case Direction::YNEG:
      v0 = { DirectX::XMFLOAT3(x - 0.5f,
                               y - 0.5f,
                               z + 0.5f),
             DirectX::XMFLOAT3(0.0f, -1.0f,  0.0f),
             col };
      v1 = { DirectX::XMFLOAT3(x + 0.5f,
                               y - 0.5f,
                               z + 0.5f),
             DirectX::XMFLOAT3(0.0f, -1.0f,  0.0f),
             col };
      v2 = { DirectX::XMFLOAT3(x + 0.5f,
                               y - 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(0.0f, -1.0f,  0.0f),
             col };
      v3 = { DirectX::XMFLOAT3(x - 0.5f,
                               y - 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(0.0f, -1.0f,  0.0f),
             col };
      break;
      // -----------------------------------------------
    case Direction::YPOS:
      v0 = { DirectX::XMFLOAT3(x - 0.5f,
                               y + 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(0.0f, +1.0f,  0.0f),
             col };
      v1 = { DirectX::XMFLOAT3(x + 0.5f,
                               y + 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(0.0f, +1.0f,  0.0f),
             col };
      v2 = { DirectX::XMFLOAT3(x + 0.5f,
                               y + 0.5f,
                               z + 0.5f),
             DirectX::XMFLOAT3(0.0f, +1.0f,  0.0f),
             col };
      v3 = { DirectX::XMFLOAT3(x - 0.5f,
                               y + 0.5f,
                               z + 0.5f),
             DirectX::XMFLOAT3(0.0f, +1.0f,  0.0f),
             col };
      break;
      // -----------------------------------------------
    case Direction::ZNEG:
      v0 = { DirectX::XMFLOAT3(x - 0.5f,
                               y - 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(0.0f,  0.0f, -1.0f),
             col };
      v1 = { DirectX::XMFLOAT3(x + 0.5f,
                               y - 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(0.0f,  0.0f, -1.0f),
             col };
      v2 = { DirectX::XMFLOAT3(x + 0.5f,
                               y + 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(0.0f,  0.0f, -1.0f),
             col };
      v3 = { DirectX::XMFLOAT3(x - 0.5f,
                               y + 0.5f,
                               z - 0.5f),
             DirectX::XMFLOAT3(0.0f,  0.0f, -1.0f),
             col };
      break;
      // -----------------------------------------------
    case Direction::ZPOS:
      v0 = { DirectX::XMFLOAT3(x + 0.5f,
                               y - 0.5f,
                               z + 0.5f),
             DirectX::XMFLOAT3(0.0f,  0.0f, +1.0f),
             col };
      v1 = { DirectX::XMFLOAT3(x - 0.5f,
                               y - 0.5f,
                               z + 0.5f),
             DirectX::XMFLOAT3(0.0f,  0.0f, +1.0f),
             col };
      v2 = { DirectX::XMFLOAT3(x - 0.5f, y + 0.5f, z + 0.5f),
             DirectX::XMFLOAT3(0.0f,  0.0f, +1.0f),
             col };
      v3 = { DirectX::XMFLOAT3(x + 0.5f, y + 0.5f, z + 0.5f),
             DirectX::XMFLOAT3(0.0f,  0.0f, +1.0f),
             col };
      break;
    }

    this->vertices.push_back(v0);
    this->vertices.push_back(v1);
    this->vertices.push_back(v2);
    this->vertices.push_back(v3);

    // Add indices
    this->indices.push_back(cur_offset_vertices + 0);
    this->indices.push_back(cur_offset_vertices + 3);
    this->indices.push_back(cur_offset_vertices + 1);

    this->indices.push_back(cur_offset_vertices + 1);
    this->indices.push_back(cur_offset_vertices + 3);
    this->indices.push_back(cur_offset_vertices + 2);

    // Add face
    NaiveFace* p_face = new NaiveFace(cur_offset_vertices,
                                      cur_offset_indices);
    this->faces.push_back(p_face);
    this->face_map[key] = p_face;
  }
}


void NaiveObjectView::removeFace(Position pos, Direction dir) {
  NaiveFaceKey key = NaiveFaceKey(pos, dir);
  try {
    NaiveFace* face = this->face_map.at(key);
    
    if (this->faces.back() != face) {
      // The face being deleted is not the last face, thus we place the last 
      // face at the position of the face to be deleted

      // Retrieve last face added
      NaiveFace* last_face = this->faces.back();

      // Replace vertices
      this->vertices[face->vert_offset + 0] = this->vertices[last_face->vert_offset + 0];
      this->vertices[face->vert_offset + 1] = this->vertices[last_face->vert_offset + 1];
      this->vertices[face->vert_offset + 2] = this->vertices[last_face->vert_offset + 2];
      this->vertices[face->vert_offset + 3] = this->vertices[last_face->vert_offset + 3];

      // Replace indices
      this->indices[face->index_offset + 0] = this->indices[last_face->index_offset + 0];
      this->indices[face->index_offset + 1] = this->indices[last_face->index_offset + 1];
      this->indices[face->index_offset + 2] = this->indices[last_face->index_offset + 2];
      this->indices[face->index_offset + 3] = this->indices[last_face->index_offset + 3];
      this->indices[face->index_offset + 4] = this->indices[last_face->index_offset + 4];
      this->indices[face->index_offset + 5] = this->indices[last_face->index_offset + 5];

      // replace data at face
      face->index_offset = last_face->index_offset;
      face->vert_offset = last_face->vert_offset;

      face = last_face;
    }

    // The last face will be deleted
    // The last entry of faces will always correspond with the last block 
    // of vertices and indices.

    // delete vertices
    for (int i = 0; i < 4; ++i) {
      this->vertices.pop_back();
    }
    // delete indices
    for (int i = 0; i < 6; ++i) {
      this->indices.pop_back();
    }

    // delete key_map entry
    this->face_map.erase(key);
    // delete face
    delete face;
    faces.pop_back();
  } catch (const std::out_of_range& e) {
    // There exists no face at the specified position
    return;
  }
}


void NaiveObjectView::setColour(Position pos, Direction dir, DirectX::XMFLOAT3 col) {
  // Retrieve face
  NaiveFaceKey key = NaiveFaceKey(pos, dir);
  NaiveFace* p_face = this->face_map.at(key);
  // Update vertex colours
  this->vertices[p_face->vert_offset + 0].color = col;
  this->vertices[p_face->vert_offset + 1].color = col;
  this->vertices[p_face->vert_offset + 2].color = col;
  this->vertices[p_face->vert_offset + 3].color = col;
}

}
}
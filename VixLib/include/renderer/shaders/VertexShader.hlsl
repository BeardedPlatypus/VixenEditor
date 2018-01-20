// ---------------------------------------------------------------------------
// Model Matrices
// ---------------------------------------------------------------------------
cbuffer CameraCB : register(b0) {
  matrix model_view;
  matrix inv_transpose_model_view;
  matrix projection;
}


// ---------------------------------------------------------------------------
// Input / Output
// ---------------------------------------------------------------------------
struct VertexShaderInput {
  float3 pos    : POSITION;
  float3 normal : NORMAL0;
  float3 color  : COLOR0;
};


struct PixelShaderInput {
  float4 pos    : SV_POSITION;
  float4 camera_pos : POSITION0;
  float3 normal : NORMAL0;
  float3 color : COLOR0;
};


// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
PixelShaderInput main(VertexShaderInput input) {
  PixelShaderInput output;
  float4 pos = float4(input.pos, 1.0f);

  // Transform the vertex position into projected space.
  pos = mul(pos, model_view);
  output.camera_pos = pos;

  pos = mul(pos, projection);
  output.pos = pos;

  // Pass the color through without modification
  float4 normal = float4(input.normal, 0.0f);
  normal = mul(normal, inv_transpose_model_view);
  output.normal = normal;
  output.color = input.color; //float3(1.0f, 1.0f, 1.0f);

	return output;
}
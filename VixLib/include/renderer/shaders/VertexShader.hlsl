// ---------------------------------------------------------------------------
// Model Matrices
// ---------------------------------------------------------------------------
cbuffer CameraCB : register(b0) {
  matrix model;
  matrix view;
  matrix projection;
}


// ---------------------------------------------------------------------------
// Input / Output
// ---------------------------------------------------------------------------
struct VertexShaderInput {
  float3 pos : POSITION;
  float3 col : COLOR0;
};


struct PixelShaderInput {
  float4 pos : SV_POSITION;
  float3 col : COLOR0;
};


// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
PixelShaderInput main(VertexShaderInput input) {
  PixelShaderInput output;
  float4 pos = float4(input.pos, 1.0f);

  // Transform the vertex position into projected space.
  pos = mul(pos, model);
  pos = mul(pos, view);
  pos = mul(pos, projection);
  output.pos = pos;

  // Pass the color through without modification
  output.col = input.col;

	return output;
}
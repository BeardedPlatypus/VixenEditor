// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput {
  float4 pos : SV_POSITION;
  float3 col : COLOR0;
};


// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
float4 main(PixelShaderInput input) : SV_TARGET {
	return float4(input.col, 1.0f);
}
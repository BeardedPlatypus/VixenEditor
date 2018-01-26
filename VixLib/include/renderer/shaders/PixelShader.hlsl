// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput {
  float4 pos : SV_POSITION;
  float4 camera_pos : POSITION0;
  float3 normal : NORMAL0;
  float3 color : COLOR0;
};


cbuffer LightingCB : register(b0) {
  float4 pos : POSITION1;
};


float calcCosAngularIncidence(float3 light_pos, float3 surface_pos, float3 normal) {
  float3 light_direction = normalize(light_pos - surface_pos);
  return clamp(dot(normal, light_direction), 0.0f, 1.0f);
}


// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
float4 main(PixelShaderInput input) : SV_TARGET{
  float3 surface_pos = input.camera_pos.xyz / input.camera_pos.w;
  float key_angular_incidence = calcCosAngularIncidence(float3(-2.048f, 2.868f, 1.453f), 
                                                        surface_pos,
                                                        input.normal);
  float fill_angular_incidence = calcCosAngularIncidence(float3(1.216f, 1.710f, 0.4616f), 
                                                         surface_pos,
                                                         input.normal);
  float back_angular_incidence = calcCosAngularIncidence(float3(0.0f, 2.5f, 9.33f), 
                                                         surface_pos,
                                                         input.normal);
  float light_val = 0.200 +                           // ambient term
                    0.600 * key_angular_incidence +   // key light
                    0.300 * fill_angular_incidence; + // fill light
                    0.250 * back_angular_incidence;   // backlight
                    
  float3 col = input.color * light_val;
  return float4(col, 1.0f);
}
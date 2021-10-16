struct VertexData
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCORD;
};


float4 color : register(b0);

[RootSignature("RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), RootConstants(num32BitConstants=4, b0)")]
float4 main(VertexData pxd) : SV_TARGET
{
	float dist = smoothstep(0.5f, 0.5f + ddx(pxd.uv.x) * 2.1f, 1.0f - length(pxd.uv));
	return float4(color.xyz * dist, 1.0f);
}

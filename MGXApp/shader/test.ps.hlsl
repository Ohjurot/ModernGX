struct VertexData
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCORD;
};


float4 color : register(b0);
Texture2D<float4> tex : register(t0);
sampler smp : register(s0);

[RootSignature("RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), RootConstants(num32BitConstants=4, b0), DescriptorTable(SRV(t0, numDescriptors = unbounded, flags = DESCRIPTORS_VOLATILE)), StaticSampler(s0)")]
float4 main(VertexData pxd) : SV_TARGET
{
	float dist = smoothstep(0.5f, 0.5f + ddx(pxd.uv.x) * 2.1f, 1.0f - length(pxd.uv));
	return float4(tex.Sample(smp, float2(pxd.uv.x + 0.5f, -pxd.uv.y + 0.5f)).rgb, dist);
}

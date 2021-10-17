struct VertexData
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCORD;
};

[RootSignature("RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), RootConstants(num32BitConstants=4, b0), DescriptorTable(SRV(t0, numDescriptors = unbounded, flags = DESCRIPTORS_VOLATILE)), StaticSampler(s0)")]
VertexData main(float4 pos : POSITION)
{
	VertexData vtx;
	vtx.pos = pos;
	vtx.uv = pos.xy;
	
	return vtx;
}

float4 color : register(b0);

[RootSignature("RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), RootConstants(num32BitConstants=4, b0)")]
float4 main() : SV_TARGET
{
	return color;
}

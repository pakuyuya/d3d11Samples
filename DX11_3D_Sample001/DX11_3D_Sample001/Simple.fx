cbuffer CBPerFrame : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
}

struct VSInput
{
	float3 Position : POSITION;
};

struct GSPSInput
{
	float4 Position : SV_POSITION;
};

GSPSInput VSFunc(VSInput input)
{
	GSPSInput output = (GSPSInput)0;

	output.Position = float4( input.Position, 1.0f );

	return output;
}

[maxvertexcount(3)]
void GSFunc(triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream)
{
	for (int i = 0; i < 3; ++i)
	{
		GSPSInput output = (GSPSInput)0;

		float4 worldPos = mul(World, input[i].Position);
		float4 viewPos = mul(View, worldPos);
		float4 projPos = mul(Proj, viewPos);

		output.Position = projPos;

		stream.Append(output);
	}
	stream.RestartStrip();
}

float4 PSFunc(GSPSInput output) : SV_TARGET0
{
	return float4(0.25, 1.0f, 0.25f, 1.0f);
}
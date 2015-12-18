//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 DiffuseMtrl;
	float4 DiffuseLight;
	float4 AmbientMaterial;
	float4 AmbientLight;
	float4 SpecularMtrl;
	float4 SpecularLight;
	float3 EyePosW;
	float SpecularPower;
	float3 LightVecW;
}
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL0;
	float4 Color : COLOR0;
	float3 NormalL : NORMAL1;
	float2 Tex : TEXCOORD0;
};
VS_OUTPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, World);

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.NormalL = NormalL;

	output.Tex = Tex;

	return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 toEye = normalize(EyePosW - input.Pos.xyz);

	float3 normalW = mul(float4(input.NormalL, 0.0f), World);
	normalW = normalize(normalW);
	float3 r = reflect(-LightVecW, normalW);

	float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);
	float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);

	float3 ambient = (AmbientMaterial * AmbientLight).rgb;

	float3 diffuse = diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb;
	float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;
	float4 Color;

	float4 textureColour = txDiffuse.Sample(samLinear, input.Tex);

	Color.rgb = (textureColour + (diffuse + ambient)) + specular;
	Color.a = DiffuseMtrl.a;

	return Color;
}
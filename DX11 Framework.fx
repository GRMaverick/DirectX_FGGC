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
cbuffer FogCBuffer
{
	float fogStart;
	float fogEnd;
}
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL0;
	float4 Color : COLOR0;
	float3 NormalL : NORMAL1;
	float2 Tex : TEXCOORD0;
	float fogFactor : FOG;
};
VS_OUTPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 Tex : TEXCOORD0)
{
	float4 cameraPosition;

	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.NormalL = NormalL;
	output.Tex = Tex;

	cameraPosition = mul(Pos, World);
	cameraPosition = mul(cameraPosition, View);
	output.fogFactor = saturate((fogEnd - cameraPosition.z) / (fogEnd - fogStart));

	return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{

	float4 colour;
	float4 textureColour = txDiffuse.Sample(samLinear, input.Tex);
	float4 fogColour;

	float3 toEye = normalize(EyePosW - input.Pos.xyz);

	float3 normalW = mul(float4(input.NormalL, 0.0f), World);
	normalW = normalize(normalW);

	float3 r = reflect(-LightVecW, normalW);

	float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);
	float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);

	float3 ambient = (AmbientMaterial * AmbientLight).rgb;
	float3 diffuse = diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb;
	float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;

	fogColour = float4(0.5f, 0.5f, 0.5f, 1.0f);

	colour.rgb = /* input.fogFactor * */((textureColour + (diffuse + ambient)) + specular) /* fogColour*/;
	colour.a = DiffuseMtrl.a;

	return colour;
}
//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
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

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);


struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
	float3 Normal : NORMAL0;
	float3 PosW : POSITION;
	float2 Tex : TEXCOORD0;
};

VS_OUTPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Normal = mul(float4(NormalL, 1.0f), World).xyz;
	output.Normal = normalize(output.Normal);

	output.PosH = mul(Pos, World);
	output.PosW = mul(Pos, View);
	output.PosW = mul(Pos, Projection);


	output.Tex = Tex;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
	input.Normal = normalize(input.Normal);

	float4 textureColour = txDiffuse.Sample(samLinear, input.Tex);

	float3 toEye = normalize(EyePosW - input.PosW.xyz);

	float3 r = reflect(-LightVecW, input.Normal);

	float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);
	float diffuseAmount = max(dot(LightVecW, input.Normal), 0.0f);

	float3 ambient = (AmbientMaterial * AmbientLight).rgb;
	float3 diffuse = diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb;
	float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;

	float4 Color;
	Color.rgb = diffuse + ambient + specular;
	Color.a = DiffuseMtrl.a;

	// Ignore Colour Calculations
	return textureColour;
}

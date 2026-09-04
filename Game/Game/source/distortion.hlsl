Texture2D SceneTexture : register(t0);
SamplerState SceneSampler : register(s0);

struct PSIinput
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR0;
    float4 Specular : COLOR1;
    float2 TexCoords0 : TEXCOORD0;
    float2 TexCoords1 : TEXCOORD1;
};

static const float DISP_SIZE_W = 1280.0f;
static const float DISP_SIZE_H = 720.0f;

static const float STRENGTH = 0.05f; // ブラーの強さを調整する値 

static const float DIRECTION_X = 1.0f;
static const float DIRECTION_Y = 0.0f; 

static const int SAMPLE_COUNT = 9; // サンプル数を調整する値

float4 main(PSIinput input) : SV_TARGET
{
    float2 uv = input.Position.xy / float2(DISP_SIZE_W, DISP_SIZE_H);
    float2 center = float2(0.5f, 0.5f); // 中心座標を設定する

    float2 direction = uv - center;

    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll] // このforループを可能なら展開して処理してください

    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        float t = (float) i / (float) (SAMPLE_COUNT - 1); // 0.0から1.0までの範囲でサンプル位置を計算する

        float offset = lerp(-1.0f, 1.0f, t); // サンプル位置を-1.0から1.0までの範囲で計算する

        float2 sampleUv = uv + direction * STRENGTH * offset; // サンプル位置を計算する

        sampleUv = saturate(sampleUv); // サンプル位置を0.0から1.0までの範囲に制限する

        // サンプル位置からテクスチャの色を取得する
        color += SceneTexture.Sample(
			SceneSampler,
			sampleUv
		);
    }

    return color / (float) SAMPLE_COUNT;
}
#include <Component/EffectRenderer.h>
#include <GameFrame/EffectManager.h>
#include <Component/Camera.h>
#include <string>
#include <Windows.h>
#include <GameFrame/RenderSystem.h>



EffectRenderer::EffectRenderer()
{
    RenderSystem::Instance().RegisterEffect(this);
}

EffectRenderer::~EffectRenderer()
{
    RenderSystem::Instance().UnregisterEffect(this);
    Stop();
}

void EffectRenderer::Draw()
{
    auto manager  = EffectManager::Instance().GetEffectManager();
    auto renderer = EffectManager::Instance().GetEffectRenderer();
 


    DirectX::XMMATRIX matrix = GetTransform()->GetWorldMatrix();
    manager->SetMatrix(m_handle, ConvertXMMatrixToEffekseerMatrix43(matrix));
    // エフェクトの描画を行う。
    Effekseer::Manager::DrawParameter drawParameter;
    drawParameter.ZNear = 0.0f;
    drawParameter.ZFar = 1.0f;
    drawParameter.ViewProjectionMatrix = renderer->GetCameraProjectionMatrix();
    manager->Draw(drawParameter);

}

void EffectRenderer::Load(const std::string& fileName)
{
    // UTF-8 → wchar_t (UTF-16)
    int size_needed =
        MultiByteToWideChar(CP_UTF8, 0, fileName.c_str(), -1, nullptr, 0);

    std::wstring wstr(size_needed, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, fileName.c_str(), -1,
        &wstr[0], size_needed);

    //ファイル読込み
    m_effect = Effekseer::Effect::Create(EffectManager::Instance().GetEffectManager(), (char16_t*)wstr.data());


}

void EffectRenderer::Play()
{

    Vector3 pos = GetTransform()->GetPosition(Space::WORLD);

    m_handle = EffectManager::Instance().GetEffectManager()->Play(m_effect, pos.x, pos.y, pos.z);

}

void EffectRenderer::Stop()
{
    EffectManager::Instance().GetEffectManager()->StopEffect(m_handle);

}


Effekseer::Matrix43 ConvertXMMatrixToEffekseerMatrix43(const DirectX::XMMATRIX& dxMatrix)
{
    //XMMATRIXを行優先の4x4浮動小数点配列に格納
    //これは、行列の各要素にアクセスするために行います。
    //dxMatrix(行, 列) の順で格納されます。
    DirectX::XMFLOAT4X4 dxFloat4x4;
    DirectX::XMStoreFloat4x4(&dxFloat4x4, dxMatrix);

    Effekseer::Matrix43 efkMatrix;

    // 2. Effekseer::Matrix43 (列優先、3x4) へ値をコピー
    //    Effekseer::Matrix43 は 4つの Effekseer::Vector3D (x, y, z, w) で構成されると仮定します。
    //    Vector3D m[4] で定義されている場合、m[0]がX軸、m[1]がY軸、m[2]がZ軸、m[3]がT(平行移動)ベクトルを表します。
    //    Effekseerの行列は列優先 (Column-Major) の慣習に従って設定します。
    //    
    //    Effekseer::Matrix43
    //      m[0].x   m[1].x   m[2].x   m[3].x  (<- Effekseerは3x4なので、実質 m[3].x はT.x)
    //      m[0].y   m[1].y   m[2].y   m[3].y
    //      m[0].z   m[1].z   m[2].z   m[3].z
    //
    //    DirectX::XMFLOAT4X4 (row, col)
    //      _11 _12 _13 _14
    //      _21 _22 _23 _24
    //      _31 _32 _33 _34
    //      _41 _42 _43 _44

    // X軸ベクトル (行1)
    efkMatrix.Value[0][0] = dxFloat4x4._11;
    efkMatrix.Value[0][1] = dxFloat4x4._12;
    efkMatrix.Value[0][2] = dxFloat4x4._13;

    // Y軸ベクトル (行2)
    efkMatrix.Value[1][0] = dxFloat4x4._21;
    efkMatrix.Value[1][1] = dxFloat4x4._22;
    efkMatrix.Value[1][2] = dxFloat4x4._23;

    // Z軸ベクトル (行3)
    efkMatrix.Value[2][0] = dxFloat4x4._31;
    efkMatrix.Value[2][1] = dxFloat4x4._32;
    efkMatrix.Value[2][2] = dxFloat4x4._33;

    // 平行移動ベクトル (行4)
    efkMatrix.Value[3][0] = dxFloat4x4._41;
    efkMatrix.Value[3][1] = dxFloat4x4._42;
    efkMatrix.Value[3][2] = dxFloat4x4._43;

    //注意: Effekseerの行列定義が異なる場合は、上記のマッピングを変更する必要があります。
    //多くのEffekseerの実装では、回転・スケール部分を転置し、平行移動部分をそのままコピーします。
    //今回の実装では、DirectXの行ベクトル (X軸、Y軸、Z軸) を Effekseerの列ベクトル (X, Y, Z) として
    //割り当てています。これにより、DirectXの行優先データがEffekseerの列優先データに**転置されて**格納されます。
    //これがDirectXとEffekseerの一般的な変換方法です。

    return efkMatrix;
}

Effekseer::Matrix44 ConvertXMMatrixToEffekseerMatrix44(const DirectX::XMMATRIX& dxMatrix)
{
    //XMMATRIXを行優先の4x4浮動小数点配列に格納
    //これは、行列の各要素にアクセスするために行います。
    //dxMatrix(行, 列) の順で格納されます。
    DirectX::XMFLOAT4X4 dxFloat4x4;
    DirectX::XMStoreFloat4x4(&dxFloat4x4, dxMatrix);

    Effekseer::Matrix44 efkMatrix;

    // X軸ベクトル (行1)
    efkMatrix.Values[0][0] = dxFloat4x4._11;
    efkMatrix.Values[0][1] = dxFloat4x4._12;
    efkMatrix.Values[0][2] = dxFloat4x4._13;
    efkMatrix.Values[0][3] = dxFloat4x4._14;
    // Y軸ベクトル (行2)
    efkMatrix.Values[1][0] = dxFloat4x4._21;
    efkMatrix.Values[1][1] = dxFloat4x4._22;
    efkMatrix.Values[1][2] = dxFloat4x4._23;
    efkMatrix.Values[1][3] = dxFloat4x4._24;
    // Z軸ベクトル (行3)
    efkMatrix.Values[2][0] = dxFloat4x4._31;
    efkMatrix.Values[2][1] = dxFloat4x4._32;
    efkMatrix.Values[2][2] = dxFloat4x4._33;
    efkMatrix.Values[2][3] = dxFloat4x4._34;
    // 平行移動ベクトル (行4)
    efkMatrix.Values[3][0] = dxFloat4x4._41;
    efkMatrix.Values[3][1] = dxFloat4x4._42;
    efkMatrix.Values[3][2] = dxFloat4x4._43;
    efkMatrix.Values[3][3] = dxFloat4x4._44;

    return efkMatrix;
}

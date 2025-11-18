//==============================================================================
//
//   カメラクラス [camera.h]
// Author : Yumito Yokozuka
// Date   : 2025/11/03
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
private:
    XMFLOAT2 m_position;   // カメラの座標
    XMFLOAT2 m_size;       // 画面のズーム用
    float m_screenR, m_screenL;//screenの左右の端
    float m_screenU, m_screenD;//screenの上下の端

public:
    Camera();

    void InitializeCamera();
    void Update(double elapsed_time);
    void SetPosition(float x, float y);
    void Move(float dx, float dy);

    XMFLOAT2 GetPosition() const { return m_position; }
};

#endif // CAMERA_H


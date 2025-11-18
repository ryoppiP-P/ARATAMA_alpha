//==============================================================================
//
//   カメラクラス [camera.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/11/03
//------------------------------------------------------------------------------
//
//==============================================================================
#include "camera.h"
#include "game.h"


Camera::Camera()
    :m_position(0.0f,0.0f), m_size(0.0f, 0.0f)
{
    m_screenR = m_position.x + SCREEN_WIDTH / 2;
    m_screenL = m_position.x - SCREEN_WIDTH / 2;
    m_screenU = m_position.y - SCREEN_HEIGHT / 2;
    m_screenD = m_position.y + SCREEN_HEIGHT / 2;
}

void Camera::InitializeCamera()
{

}

void Camera::Update(double elapsed_time)
{
    m_screenR = m_position.x + SCREEN_WIDTH / 2;
    m_screenL = m_position.x - SCREEN_WIDTH / 2;
    m_screenU = m_position.y - SCREEN_HEIGHT / 2;
    m_screenD = m_position.y + SCREEN_HEIGHT / 2;


    // ターゲットを中心に表示する
    m_position.x -= (SCREEN_WIDTH / 2.0f);
    m_position.y -= (SCREEN_HEIGHT / 2.0f);
}

void Camera::SetPosition(float x, float y)
{
    m_position = { x, y };
}

void Camera::Move(float dx, float dy)
{
    m_position.x += dx;
    m_position.y += dy;
}

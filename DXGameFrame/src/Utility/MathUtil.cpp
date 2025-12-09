//MathUtil.cpp
#include <Utility/MathUtil.h>
#include <math.h>

bool MathUtil::Approximately(float a, float b)
{
    //相対誤差許容値を求める
    float relEps = Math::EPS * fmaxf(fabsf(a), fabsf(b));

    //相対・絶対誤差許容値のどちらかに収まれば同じとみなす
    return fabsf(b - a) < fmaxf(relEps, Math::ZERO_EPS);
}

float MathUtil::ClampAngle(float angle, float min, float max)
{
    //角度正規化
    angle = NormalizeAngle(angle);
    min = NormalizeAngle(min);
    max = NormalizeAngle(max);

    //360を跨ぐ範囲かチェック
    if (min < max)
    {
        //範囲内かチェック
        if (angle < max && angle > min)
            return angle;
    }
    else
    {
        //範囲内かチェック
        if (angle < max || angle > min)
            return angle;
    }

    // どちらに近いかで吸着
    float toMin = AngleDistance(angle, min);
    float toMax = AngleDistance(angle, max);

    if (toMin < toMax)
    {
        return min;
    }
    else
    {
        return max;
    }
}

float MathUtil::NormalizeAngle(float angle)
{
    //0～360の範囲に収める
    angle = fmodf(angle, 360.0f);
    if (angle < 0.0f)
        angle += 360.0f;
    return angle;
}

float MathUtil::AngleDistance(float angle_a, float angle_b)
{
    //角度間の距離を求める
    float diff = fabsf(NormalizeAngle(angle_a - angle_b));
    if (diff < 180.0f)
    {
        return diff;
    }
    else
    {
        return 360.0f - diff;
    }
}

float MathUtil::DegToRad(float deg)
{
    return deg * (Math::TAU / 360.0f);
}

float MathUtil::RadToDeg(float rad)
{
    return rad * (360.0f / Math::TAU);
}

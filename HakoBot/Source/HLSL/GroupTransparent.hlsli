#ifndef INCLUDE_GROUP_TRANSPARENT
#define INCLUDE_GROUP_TRANSPARENT

Texture2D<float> transparentDepthMap : register(t10);

void CheckDepth(float4 pos : SV_POSITION)
{
    int3 pixelCoord = int3(pos.xy, 0);
    float nearestDepth = transparentDepthMap.Load(pixelCoord);
    
    float myDepth = pos.z;
   
    float epsilon = 0.00001f;
    
    if (myDepth > nearestDepth + epsilon)
    {
        discard;
    }
}

#endif
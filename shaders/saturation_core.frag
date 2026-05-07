#version 140
// Saturation shader - AMD linear and NVIDIA vibrance modes

uniform sampler2D sampler;
uniform vec4 modulation;
uniform float saturationAmount;  // 1.0 = normal, 1.5 = 150%, 2.0 = 200%
uniform int mode;                // 0 = AMD (linear), 1 = NVIDIA (non-linear vibrance)
in vec2 texcoord0;
out vec4 fragColor;

// BT.709 luminance coefficients (for linear RGB)
const vec3 lumaCoeff = vec3(0.2126, 0.7152, 0.0722);

// sRGB to linear conversion
vec3 srgbToLinear(vec3 color) {
    bvec3 isLow = lessThanEqual(color, vec3(0.04045));
    vec3 loPart = color / 12.92;
    vec3 hiPart = pow((color + 0.055) / 1.055, vec3(2.4));
    return mix(hiPart, loPart, isLow);
}

// Linear to sRGB conversion
vec3 linearToSrgb(vec3 color) {
    bvec3 isLow = lessThanEqual(color, vec3(0.0031308));
    vec3 loPart = color * 12.92;
    vec3 hiPart = pow(color, vec3(1.0 / 2.4)) * 1.055 - 0.055;
    return mix(hiPart, loPart, isLow);
}

// AMD-style linear saturation
vec3 amdSaturation(vec3 color, float amount) {
    float luma = dot(color, lumaCoeff);
    return vec3(luma) + amount * (color - vec3(luma));
}

// NVIDIA-style non-linear vibrance
// Boosts less saturated colors more, leaves already saturated colors alone
vec3 nvidiaVibrance(vec3 color, float amount) {
    float luma = dot(color, lumaCoeff);

    // Calculate current saturation (0 = gray, 1 = fully saturated)
    float maxC = max(max(color.r, color.g), color.b);
    float minC = min(min(color.r, color.g), color.b);
    float currentSat = (maxC - minC) / max(maxC, 0.001);

    // Non-linear adjustment: less saturated colors get boosted more
    // The (1 - currentSat) factor reduces the effect on already saturated colors
    float adjustedAmount = 1.0 + (amount - 1.0) * (1.0 - currentSat * 0.5);

    return vec3(luma) + adjustedAmount * (color - vec3(luma));
}

void main()
{
    vec4 tex = texture(sampler, texcoord0);

    // Handle premultiplied alpha
    float alpha = max(tex.a, 0.001);
    tex.rgb /= alpha;

    // Convert sRGB to linear for proper color math
    vec3 linear = srgbToLinear(tex.rgb);

    // Apply saturation based on mode
    vec3 saturated;
    if (mode == 1) {
        saturated = nvidiaVibrance(linear, saturationAmount);
    } else {
        saturated = amdSaturation(linear, saturationAmount);
    }

    // Clamp to valid range
    saturated = clamp(saturated, 0.0, 1.0);

    // Convert back to sRGB
    vec3 result = linearToSrgb(saturated);

    // Re-apply premultiplied alpha
    result *= alpha;

    fragColor = vec4(result, tex.a) * modulation;
}

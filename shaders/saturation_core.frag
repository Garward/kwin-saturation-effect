#version 140
// Saturation shader - direct formula implementation

uniform sampler2D sampler;
uniform vec4 modulation;
uniform float saturationAmount;  // 1.0 = normal, 1.5 = 150%, 2.0 = 200%
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

void main()
{
    vec4 tex = texture(sampler, texcoord0);

    // Handle premultiplied alpha
    float alpha = max(tex.a, 0.001);
    tex.rgb /= alpha;

    // Convert sRGB to linear for proper color math
    vec3 linear = srgbToLinear(tex.rgb);

    // Calculate luminance
    float luma = dot(linear, lumaCoeff);

    // Apply saturation: output = luma + S * (input - luma)
    vec3 saturated = vec3(luma) + saturationAmount * (linear - vec3(luma));

    // Clamp to valid range
    saturated = clamp(saturated, 0.0, 1.0);

    // Convert back to sRGB
    vec3 result = linearToSrgb(saturated);

    // Re-apply premultiplied alpha
    result *= alpha;

    fragColor = vec4(result, tex.a) * modulation;
}

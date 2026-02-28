// Saturation shader - direct formula implementation (legacy)

uniform sampler2D sampler;
uniform vec4 modulation;
uniform float saturationAmount;  // 1.0 = normal, 1.5 = 150%, 2.0 = 200%
varying vec2 texcoord0;

// BT.709 luminance coefficients (for linear RGB)
const vec3 lumaCoeff = vec3(0.2126, 0.7152, 0.0722);

// sRGB to linear conversion
vec3 srgbToLinear(vec3 color) {
    vec3 loPart = color / 12.92;
    vec3 hiPart = pow((color + 0.055) / 1.055, vec3(2.4));
    return vec3(
        color.r <= 0.04045 ? loPart.r : hiPart.r,
        color.g <= 0.04045 ? loPart.g : hiPart.g,
        color.b <= 0.04045 ? loPart.b : hiPart.b
    );
}

// Linear to sRGB conversion
vec3 linearToSrgb(vec3 color) {
    vec3 loPart = color * 12.92;
    vec3 hiPart = pow(color, vec3(1.0 / 2.4)) * 1.055 - 0.055;
    return vec3(
        color.r <= 0.0031308 ? loPart.r : hiPart.r,
        color.g <= 0.0031308 ? loPart.g : hiPart.g,
        color.b <= 0.0031308 ? loPart.b : hiPart.b
    );
}

void main()
{
    vec4 tex = texture2D(sampler, texcoord0);

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

    gl_FragColor = vec4(result, tex.a) * modulation;
}

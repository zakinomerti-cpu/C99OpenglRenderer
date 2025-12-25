#version 120
varying vec3 vNormal;
varying vec2 vTexCoord;
uniform sampler2D texSampler;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

#define OCTAVES 6
float fbm (in vec2 st) {
    // Initial values
    float value = 0.0;
    float amplitude = .5;
    float frequency = 0.;
    //
    // Loop of octaves
    for (int i = 0; i < OCTAVES; i++) {
        value += amplitude * noise(st);
        st *= 2.;
        amplitude *= .5;
    }
    return value;
}

void main() {

    vec2 uv = vTexCoord;
    vec2 p = uv - 0.5;
    float r = length(p);
    if (r > 0.50) discard;
    float radial = smoothstep(0.50, 0.0, r);
    float s = fbm(uv * 6.0);
    vec3 color = vec3(1.0, 0.6, 0.1);
    color += s * vec3(1.0, 0.3, 0.0);
    color *= radial * 2.0;

    gl_FragColor = vec4(color, color.r);
}

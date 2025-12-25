#version 120
varying vec3 vNormal;
varying vec2 vTexCoord;

float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a,b,u.x) + (c - a)*u.y*(1.0 - u.x) + (d - b)*u.x*u.y;
}

#define OCTAVES 6
float fbm (in vec2 st) {
    float value = 0.0;
    float amplitude = .5;
    for(int i = 0; i < OCTAVES; i++) {
        value += amplitude * noise(st);
        st *= 2.0;
        amplitude *= 0.5;
    }
    return value;
}

void main() {

    vec2 uv = vTexCoord;
    vec2 p  = uv - 0.5;
    float r = length(p);
    if (r > 0.50) discard;
    float terrain = fbm(uv * 4.0);
    float craters = fbm(uv * 1.5);
    float craterMask = smoothstep(0.6, 0.3, craters);
    vec3 rock_dark  = vec3(0.15, 0.13, 0.12);
    vec3 rock_light = vec3(0.32, 0.30, 0.28);

    vec3 color = mix(rock_dark, rock_light, terrain);
    color *= mix(1.0, 0.55, craterMask);
    float edge = smoothstep(0.50, 0.40, r);
    color *= edge;

    gl_FragColor = vec4(color*2., 1.0);
}

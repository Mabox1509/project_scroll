#version 330 core
out vec4 FragColor;

/*in vec3 color;*/
in vec2 texCoord;
uniform sampler2D tex0;
uniform sampler2D tex_dither;

uniform vec2 resolution;
uniform float color_depth;
uniform float brightness;


//[FUNCTIONS]
float Lerp(float _a, float _b, float _t)
{
    return _a + (_b - _a) * _t;
}

vec2 GetTiled()
{
    vec2 _uv = texCoord;
    _uv.x = floor(_uv.x * resolution.x);
    _uv.y = floor(_uv.y * resolution.y);

    return mod(_uv, 8);
}
float CalculateChannel(float _raw, vec2 _uv)
{
    vec2 _texel = vec2(1.0f / 64.0f, 1.0f / 8.0f);

    //CALCULATE VALUES
    float _a = floor(_raw / color_depth) * color_depth;
    float _b = _a + color_depth;
    float _lerp = mod(_raw,  color_depth) / color_depth;


    //CALCULATE DITHER - 7 Frames
    float _frame = floor(_lerp * 6) * 8;
    vec2 _pixuv = _uv;
    _pixuv.x += _frame;



    return Lerp(_a, _b, texture(tex_dither, _pixuv * _texel).x);
}

//[MAIN]
void main()
{
    vec4 _raw = texture(tex0, texCoord) + brightness;
    vec2 _tiled_uv = GetTiled();

    vec4 _color = vec4(CalculateChannel(_raw.x, _tiled_uv), CalculateChannel(_raw.y, _tiled_uv), CalculateChannel(_raw.z, _tiled_uv), 1.0f); //vec4(GetTiled(), 0.0f, 1.0f);
    FragColor = _color;
}
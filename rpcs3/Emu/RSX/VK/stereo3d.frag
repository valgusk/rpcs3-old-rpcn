#version 420

layout(set=0, binding=1) uniform sampler2D fs0;
layout(set=0, binding=2) uniform sampler2D fs1;
layout(location=0) in vec2 tc0;
layout(location=0) out vec4 ocol;

layout(push_constant) uniform static_data
{
 float gamma;
 int limit_range;
 int stereo;
 int stereo_image_count;
};

vec4 read_source()
{
 if (stereo == 0) return texture(fs0, tc0);

 vec2 tc0stretch = tc0 * vec2(1.f, 2.f);

 if(tc0.y > 0.5f) {
   tc0stretch -= vec2(0.f, 1.f);
 }

 vec4 left, right;
 if (stereo_image_count == 2)
 {
   left = texture(fs0, tc0stretch);
   right = texture(fs1, tc0stretch);
 }
 else
 {
   vec2 coord_left = tc0stretch * vec2(1.f, 0.4898f);
   vec2 coord_right = coord_left + vec2(0.f, 0.510204f);
   left = texture(fs0, coord_left);
   right = texture(fs0, coord_right);
 }

 return tc0.y > 0.5f ? right : left;
}

void main()
{
 vec4 color = read_source();
 color.rgb = pow(color.rgb, vec3(gamma));
 if (limit_range > 0)
   ocol = ((color * 220.) + 16.) / 255.;
 else
   ocol = color;
}
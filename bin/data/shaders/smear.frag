
uniform sampler2DRect tex;
uniform vec2 u_topleft;
uniform vec2 u_bottomright;

void main()
{
    vec3 coord_color = vec3( gl_TexCoord[0].x, 0.5, gl_TexCoord[0].y );
    vec2 coord = mix( u_topleft, u_bottomright, gl_TexCoord[0].xy );
    vec3 vid_color = texture2DRect(tex, coord).rgb;
    gl_FragColor = vec4( vid_color, 1.0 );
}

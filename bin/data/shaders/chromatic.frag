uniform sampler2DRect tex; // passed in from openFrameworks
uniform vec2 randomVals; // passed in from openFrameworks

void main() {
    
    vec4 coords = gl_TexCoord[0];

	
    // offset red
    vec4 fbo1 = texture2DRect(tex, coords.xy - randomVals);
    // keep green where it is
    vec4 fbo2 = texture2DRect(tex, coords.xy);
    // offset blue
    vec4 fbo3 = texture2DRect(tex, coords.xy + randomVals);
    
    // FBO channels mixed (incl. offsets)
    vec4 colFinal = vec4(fbo1.r, fbo2.g, fbo3.b, 1.);
    
    // Output final pixel color
    gl_FragColor = colFinal;
}

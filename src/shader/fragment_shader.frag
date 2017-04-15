/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

uniform sampler2D texture;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	vec4 originColor = gl_Color * pixel;
    // multiply it by the color
    if (originColor.g > 0.5f)
    {
    	originColor.g = 1.f;
    } else
    {
    	originColor.g = 0.f;
    }
    
    gl_FragColor = originColor;
}

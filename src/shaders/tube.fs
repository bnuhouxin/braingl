#version 330

#include colormap_fs
#include textures_fs
#include uniforms_fs
#include peel_fs

in float v_sparam; // s parameter of texture [-1..1]
in float v_tangent_dot_view;

in float v_discard;
uniform float u_fibGrowth;


void main()
{
    if ( v_index > u_fibGrowth )
    {
        discard;
    }

    vec4 color = vec4( 0.0, 0.0, 0.0, 1.0 ); 
    if ( u_colorMode == 3 )
    {
        color = texColor( v_texcoord );
	    
	    if ( !( color.r + color.g + color.b > 0.0 ) ) discard;
    }
    else if ( u_colorMode == 4 )
    {
        color = colormap( v_extra, u_colormap, u_lowerThreshold, u_upperThreshold, u_selectedMin, u_selectedMax );
 
    }
    else
    {
        color = frontColor;
    }
    float view_dot_normal = sqrt( 1. - v_sparam * v_sparam ) + .1;    
    color = clamp( view_dot_normal * ( color + 0.15 * pow( view_dot_normal, 10. ) * pow( v_tangent_dot_view, 10. ) ), 0., 1. );
    color.a = 1.0;
    
    writePeel( color.rgb );
}

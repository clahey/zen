#include "colors.inc"
global_settings { max_trace_level 256 }
#declare Magnet = sphere {
    <0, 0, 0> .97
    pigment {
	gradient x
	color_map {
	    [ 0.5 Blue transmit .1 ]
	    [ 0.5 White transmit .1 ]
	}
	scale 2
	translate -1 * x
    }
    finish {
	phong 0.9
	phong_size 200
	metallic
	reflection {
	    .2
	    metallic .5
	}
	ambient .3
    }
    interior{ior 1.5}
}

camera {
    location <.007, .016, -.016>
    look_at  <0.005, 0.005,  0.005>
}

light_source { <2, 4, -3> color White}

plane {
    <0, 1, 0>, -.01
    pigment {
	checker color Red, color Black
	scale 0.01
    }
    finish {
	reflection {
	    .5
	}
    }
}
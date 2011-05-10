#include "colors.inc"
global_settings { max_trace_level 256 }
#declare Magnet = sphere {
    <0, 0, 0> .97
    pigment {
	gradient x
	color_map {
	    [ 0.5 Blue ]
	    [ 0.5 White ]
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
}

camera {
    location <.001, .014, -.014>
    look_at  <0, 0.005,  0>
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
#include "colors.inc"
#declare Magnet = sphere {
    <0, 0, 0> .5
    pigment {
	gradient x
	color_map {
	    [ 0.5 Blue ]
	    [ 0.5 White ]
	}
	translate -0.5 * x
    }
    finish {
	phong 0.9
	phong_size 200
	metallic
	reflection {
	    .2
	    metallic .5
	}
    }
}

camera {
    location <.001, .01, -.01>
    look_at  <.0025, 0,  0>
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
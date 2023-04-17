#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Vec3 direction, Vec3 color, float brightness) : 
	direction(direction), 
	color(color), 
	brightness(brightness) { }

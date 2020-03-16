#pragma once

#include "main.hpp"

class Camera {
private:
    // Perspective controls
    float fov;      // Field of View Angle (degrees)
    float aspect;   // Aspect Ratio
    float nearClip; // Near clipping plane distance
    float farClip;  // Far clipping plane distance

    // Polar controls
    float distance; // Distance of the camera eye position to the origin (meters)
    float azimuth;  // Rotation of the camera eye position around the Y axis (degrees)
    float incline;  // Angle of the camera eye position over the XZ plane (degrees)

    // Computed data
    glm::mat4 viewProjectMtx;
    
public:
	Camera();

	void update();
	void reset();

	// Access functions
	void setAspect(float a)					{ aspect = a; }
	void setDistance(float d)				{ distance = d; }
	void setAzimuth(float a)				{ azimuth = a; }
	void setIncline(float i)				{ incline = i; }

	float getDistance()						{ return distance; }
	float getAzimuth()						{ return azimuth; }
	float getIncline()						{ return incline; }

	const glm::mat4 &getViewProjectMtx()	{ return viewProjectMtx; }
};

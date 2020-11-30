// Simple tool to find the location of a desired colored object in an image frame

#ifndef SEARCH_HH
#define SEARCH_HH

// Imports OpenCV utilities
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// Finds the direction of the color relative to the frame of reference.
//
// Params:
// pic          - The frame to analyze (NOTE: HSV COLOR SPACE)
// fov          - The field of view of the camera (NOTE: RADIANS)
// hue          - The HSV hue of the color to target
// tolerance    - The HSV tolerance of the colored object
//
// Returns:
// The angle (in radians) of the center of the colored object in frame left or
// right of the center of the image.
//
// -- RETURN NOTES:
// -- -- If the object is in the center of the frame, the returned angle is 0.0f
//
// -- -- If the object is to the left or right, it would return either a negative
// -- -- -- or positive angle relative to zero within the frame's FOV.
//
// -- -- If the color is not found, this returns 666.0f
float get_direction_of_color(cv::Mat pic, float fov, int hue, int tolerance);

// If the color is in the camera frame, this will return the distance to get to
// said color.
//
// Params:
// pic              - The frame to analyze (NOTE: HSV COLOR SPACE)
// fov              - The field of view of the camera (NOTE: RADIANS)
// hue              - The HSV hue of the color to target
// tolerance        - The HSV tolerance of the colored object
// object_height    - The known height of the colored object (in meters)
//
// Returns:
// The distance of the object (in meters) away from the camera.
// If no image is found, this returns -1.0f.
float get_distance_from_color(cv::Mat pic, float fov, int hue, int tolerance, float object_height);

#endif


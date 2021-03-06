# Color Finder
This is a simple tool using OpenCV to find the direction of a colored object in an image as well as its distance

This library has 2 functions...

## float get_direction_of_color(cv::Mat pic, float fov, int hue, int tolerance)
Finds the direction of the color relative to the frame of reference.
  
**Params**
```
  pic          - The frame to analyze (NOTE: HSV COLOR SPACE)
  fov          - The field of view of the camera (NOTE: RADIANS)
  hue          - The HSV hue of the color to target
  tolerance    - The HSV tolerance of the colored object
```

**Returns**
```
  The angle (in radians) of the center of the colored object in frame left or
  right of the center of the image.
```

*RETURN NOTES*
```
  If the object is in the center of the frame, the returned angle is 0.0f
  
  If the object is to the left or right, it would return either a negative
  -- or positive angle relative to zero within the frame's FOV.
  
  If the color is not found, this returns 666.0f
```

## float get_distance_from_color(cv::Mat pic, float fov, int hue, int tolerance, float object_height)
If the color is in the camera frame, this will return the distance to get to
said color.

**Params**
```
  pic              - The frame to analyze (NOTE: HSV COLOR SPACE)
  fov              - The field of view of the camera (NOTE: RADIANS)
  hue              - The HSV hue of the color to target
  tolerance        - The HSV tolerance of the colored object
  object_height    - The known height of the colored object (in meters)
```

**Returns**
```
  The distance of the object (in meters) away from the camera.
  If no image is found, this returns -1.0f.
```

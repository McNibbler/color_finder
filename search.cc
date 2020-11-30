// Implementation of search.hh interface

/////////////
// IMPORTS //
/////////////
#include "search.hh"
#include <iostream>


////////////
// MACROS //
////////////

// Clamps between 2 values
#define clamp(min, x, max) (((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x)))


///////////////
// CONSTANTS //
///////////////

#define color_pixel_threshold  5
#define image_search_edge     10
int search_count = 1;
bool DEBUG = false;


//////////////////////
// ADDITIONAL STUBS //
//////////////////////

// CV helpers
cv::Mat _make_color_mask(cv::Mat pic, int hue, int tolerance);
int _get_column_of_color(cv::Mat pic, int& row_out, int& col_out);
int _color_pixel_height(int column_index, cv::Mat pic);
float _get_visual_angle(int pixel_height, cv::Mat pic, float fov);


//////////////////////////////
// INTERFACE IMPLEMENTATION //
//////////////////////////////

// Finds the direction of the colored object relative to the current angle
float get_direction_of_color(cv::Mat pic, float fov, int hue, int tolerance) {

    if (pic.empty()){
        std::cout << "IMAGE IS NULL" << std::endl;
        return 666.0;
    }
  
    // Gets a mask of the image filtering the color
    cv::Mat color_mask = _make_color_mask(pic, hue, tolerance);
    
    int row_out = 0;
    int col_out = 0;
    
    // Finds the location of the topmost pixel of the color (which should also be the center)
    int column = _get_column_of_color(color_mask, row_out, col_out);
    
    // Returns a sentinel if the color is not in frame
    if (column < 0) {
        cv::imshow("raw", pic);
        cv::imshow("mask", color_mask);
        cv::waitKey(1);
        return 666.0;
    }

    // Finds the angle of this column
    int pic_width = pic.cols;
    float proportion = static_cast<float>(column - (pic_width / 2.0)) / static_cast<float>(pic_width / 2.0);
    float direction = proportion * (fov / 2.0);
    
    if (DEBUG) {
        for (int i = 0; i < color_mask.rows; i++) {
            uchar *ptr = color_mask.ptr<uchar>(i);
            ptr[col_out] = 128;
        }
    
        uchar *ptr = color_mask.ptr<uchar>(row_out);
        for (int j = 0; j < color_mask.cols; j++) {
            ptr[j] = 64;
        }
        
        cv::imshow("raw", pic);
        cv::imshow("mask", color_mask);
        cv::waitKey(1);
    }
    return direction;
}

// Finds the distance a color is away given the current picture
float get_distance_from_color(cv::Mat pic, float fov, int hue, int tolerance, float object_height) {
    
    // Gets a mask of the image filtering the color
    cv::Mat color_mask = _make_color_mask(pic, hue, tolerance);
    
    int a = 0, b = 0;
    
    // Finds the location of the topmost pixel of the color (which should also be the center)
    int column = _get_column_of_color(color_mask, a, b);

    // Returns a sentinel if the color is not in frame
    if (column < 0) {
        return -1.0;
    }

    // Check column for height of pixels of color
    int color_pix = _color_pixel_height(column, color_mask);

    // With height of color in pixels, find visual angle
    float visual_angle = _get_visual_angle(color_pix, pic, fov);
    
    // With visual angle of color, extrapolate distance with size of color
    float distance = (static_cast<float>(object_height) / 2.0) / tan(visual_angle / 2.0);

    // We did it reddit
    return distance;
}


////////////////////////////
// HELPERS IMPLEMENTATION //
////////////////////////////

// Makes a mask of the picture that picks out the color specifically
// Uses HSV to pick out the orange
cv::Mat _make_color_mask(cv::Mat pic, int hue, int tolerance) {
    cv::Mat frame_hsv = pic;
    
    if (pic.empty()) {
      return pic;
    }
    
    cv::Scalar low = cv::Scalar(clamp(0, hue - tolerance, 180), 128, 50);
    cv::Scalar high = cv::Scalar(clamp(0, hue + tolerance, 180), 255, 255);

    // Creates a black and white mask of the image filtered by the color color in
    // accordance to the above scalar tolerances
    cv::Mat mask;
    cv::inRange(frame_hsv, low, high, mask);
    
    return mask;
    
}

// Returns the column within the picture that indersects the middle of the color.
// Returns -1 if there is no mask in frame (i.e. all black)
int _get_column_of_color(cv::Mat pic, int& row_out, int& col_out ) {
    float col_tot = 0;
    float row_tot = 0;
    float count = 0;
    
    // Becasue the object is a circle, the highest up row that has a pixel will subsequently
    // contain the pixel that will be roughly in the direct center of the color, and can
    // be assumed as the middle
    for (int row = image_search_edge; row < pic.rows - image_search_edge; row++) {
        uchar* ptr2 = pic.ptr<uchar>(row);
        for (int column = image_search_edge; column < pic.cols - image_search_edge; column++) {
            // Found the first highest-up pixel of the mask
            if (ptr2[column]) {
                col_tot += column;
                row_tot += row;
                count += 1;
            }
        }
    }
    
    col_out = std::floor(col_tot / count);
    row_out = std::floor(row_tot / count);
    
    if (count > color_pixel_threshold) {
        return col_out;
    }
    
    return -1;
}


// Given the index of the column where the color is in the pic, 
int _color_pixel_height(int column_index, cv::Mat pic) {

    int color_pix = 0;
    for (int row = 0; row < pic.rows; row++) {
        cv::Vec3b color = pic.at<cv::Vec3b>(cv::Point(column_index, row));

        if (color.val[0] && color.val[1] && color.val[2]) {
                color_pix++;
        }
    }
    return color_pix;
}


// Given the height of an object in pixels, return the visual angle
float _get_visual_angle(int pixel_height, cv::Mat pic, float fov) {
    float proportion = static_cast<float>(pixel_height) / static_cast<float>(pic.rows);
    float visual_angle = proportion * fov;
    return visual_angle;
}






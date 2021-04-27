//
//  filters.hpp
//  
//
//  Created by Amit Mulay on 1/25/21.
//

#ifndef filters_hpp
#define filters_hpp

#include <opencv2/core/mat.hpp>

namespace custom_filters {

int blur5x5( cv::Mat &src, cv::Mat &dst );
int castStoC( cv::Mat &dst, cv::Mat &dst16 );
int sobelX3x3( cv::Mat &src, cv::Mat &dst );
int sobelY3x3( cv::Mat &src, cv::Mat &dst );
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst );
int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels );
int cartoon( cv::Mat &src, cv::Mat&dst, int levels, int magThreshold );
int pencilSketch(cv::Mat &src, cv::Mat &dst);

}


#endif /* filters_hpp */

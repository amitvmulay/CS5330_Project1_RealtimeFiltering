//
//  vidDisplay.cpp
//
//
//  Created by Amit Mulay on 1/25/21.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include "filters.hpp"



#include <iostream>

using namespace cv;
using namespace custom_filters;

enum imageMode
{
    REGULAR = 1,
    GRAYSCALE = 2,
    BLURRED = 3,
    SOBELX = 4,
    SOBELY = 5,
    SOBELXY = 6,
    MAGNITUDE = 7,
    QUANTIZE = 8,
    CARTOONIZE = 9,
    PENCIL = 10,
    CANNY = 11
}   imMode;



int main(int argc, char *argv[]) {
    cv::VideoCapture *capdev;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if( !capdev->isOpened() ) {
            printf("Unable to open video device\n");
            return(-1);
    }

    // get some properties of the image
    cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                   (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    
    //Menu buttons:
    std::cout << "Press following keys to select the effect:" << std::endl;
    std::cout << "1)Regular Image \t\t -key 'r' :" << std::endl;
    std::cout << "2)Grayscale Image \t\t -key 'g' :" << std::endl;
    std::cout << "3)Blurred Image \t\t -key 'b' :" << std::endl;
    std::cout << "4)Sobel in x direction \t\t -key 'x' :" << std::endl;
    std::cout << "5)Sobel in y direction \t\t -key 'y' :" << std::endl;
    std::cout << "6)Sobel Magnitude \t\t -key 'm' :" << std::endl;
    std::cout << "7)Burred and Quantized Image \t -key 'l' :" << std::endl;
    std::cout << "\tUse key '[' to decrease and key ']' to increase quantize level. Default level is set to 15." << std::endl;
    std::cout << "8)Cartoonized Image \t\t -key 'c' :" << std::endl;
    std::cout << "\tUse key '[' to decrease and key ']' to increase quantize level. Default level is set to 15." << std::endl;
    std::cout << "\tUse key '-' to decrease and key '=' to increase magnitude threshold. Default level is set to 20." << std::endl;
    std::cout << "9)Pencil Sketch Image \t\t -key 'p' :" << std::endl;
    std::cout << "10)Canny edge detection \t -key 'k' :" << std::endl;
    std::cout << "11)Quit \t\t\t -key 'q' :" << std::endl;
    
    

    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;
    cv::Mat dst;
    cv::Mat dst_gray;
    *capdev >> frame;
    cv::Mat dst16(frame.rows,frame.cols,CV_16SC3);
    cv::Mat mag16(frame.rows,frame.cols,CV_16SC3);
    cv::Mat tmp16(frame.rows,frame.cols,CV_16SC3);
    cv::Mat magFloatTheta(frame.rows,frame.cols,CV_32FC3);
    dst = frame.clone();
    
    int quantizeLevel = 15;
    int magThreshold = 15;
    for(;;) {
            *capdev >> frame; // get a new frame from the camera, treat as a stream
            if( frame.empty() )
            {
              printf("frame is empty\n");
              break;
            }
            char key = cv::waitKey(10);
        
            if (key == 'g')
                imMode = GRAYSCALE;
            else if(key == 'r')
                imMode = REGULAR;
            else if(key == 'b')
                imMode = BLURRED;
            else if(key == 'x')
                imMode = SOBELX;
            else if(key == 'y')
                imMode = SOBELY;
            else if(key == 'm')
                imMode = MAGNITUDE;
            else if(key == 'l')
            {
                imMode = QUANTIZE;
                quantizeLevel = 15;
            }
            else if(key == 'c')
            {
                imMode = CARTOONIZE;
                quantizeLevel = 15;
                magThreshold = 20;
            }
            else if(key == 'p')
                imMode = PENCIL;
            else if(key == '[')
                quantizeLevel--;
            else if(key == ']')
                quantizeLevel++;
            else if(key == '-')
                magThreshold--;
            else if(key == '=')
                magThreshold++;
            else if(key == 'k')
                imMode = CANNY;
        
        
            if(imMode == REGULAR)
                cv::imshow("Video", frame);
            else if (imMode == GRAYSCALE)
            {
                cvtColor(frame, dst, COLOR_RGBA2GRAY, 0);
                cv::imshow("Video", dst);
            }
            else if (imMode == BLURRED)
            {
                custom_filters::blur5x5(frame,dst);
                cv::imshow("Video", dst);
            }
            else if (imMode == SOBELX)
            {
                custom_filters::sobelX3x3(frame,dst16);
                custom_filters::castStoC(dst,dst16);
                cv::imshow("Video", dst);
            }
            else if (imMode == SOBELY)
            {
                custom_filters::sobelY3x3(frame,dst16);
                custom_filters::castStoC(dst,dst16);
                cv::imshow("Video", dst);
            }
            else if (imMode == MAGNITUDE)
            {
                custom_filters::sobelX3x3(frame,dst16);
                custom_filters::sobelY3x3(frame,tmp16);
                custom_filters::magnitude(dst16,tmp16,dst);
                cv::imshow("Video", dst);
            }
            else if (imMode == QUANTIZE)
            {
                custom_filters::blurQuantize(frame,dst,quantizeLevel);
                cv::imshow("Video", dst);
            }
            else if (imMode == CARTOONIZE)
            {
                custom_filters::cartoon(frame,dst,quantizeLevel,magThreshold);
                cv::imshow("Video", dst);
            }
            else if (imMode == PENCIL)
            {
                custom_filters::pencilSketch(frame, dst);
                cv::imshow("Video", dst);
            }
            else if (imMode == CANNY)
            {
                cvtColor(frame, dst_gray, COLOR_RGBA2GRAY, 0);
                cv::GaussianBlur(dst_gray,dst_gray,cv::Size(5, 5),1.5);
                cv::imshow("Video", dst_gray);
                cv::Canny(dst_gray,dst_gray,100,200);
                cv::imshow("Video", dst_gray);
            }
            else
                cv::imshow("Video", frame);
        
            // see if there is a waiting keystroke
            
            if( key == 'q') {
                break;
            }
    }

    delete capdev;
    return(0);
}

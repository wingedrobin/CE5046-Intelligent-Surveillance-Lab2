#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv ;

int main( int , char** )
{
	VideoCapture cap( "M01.mp4" ) ;								//Video capturing from video file.

	//Check the capturing is succeeded or not.
	if( !cap.isOpened( ) )
	{
		return -1 ;
	}

	namedWindow( "Show" , 1 ) ;									//Create a window for display the video.
	namedWindow( "Foreground" , 1 ) ;							//Create a window for display the foreground video.

	Mat capture , foreground ;									//Declaration of capture and foreground capture.
	BackgroundSubtractorMOG mog ;

	int nowFrame = 1 ;													//Declaration a integer variable as 1st frame.
	int totalFrame = ( int )cap.get( CV_CAP_PROP_FRAME_COUNT ) ;		//Get the total frame number of the video.

	char buffer[ 10 ] ;											//Declaration of a nameing buffer.
	string fileName ;											//Declaration of file name variable in string type.

	//Video processing section.
	while( nowFrame ++ != totalFrame )
	{
		//Terminal the program when key pressed.
		if( waitKey( 30 ) >= 0 )
		{
			break ;
		}

		cap >> capture ;										//Get a new frame from video capture.
		imshow( "Show" , capture ) ;							//Show the captured image from video.

		//Extract the foreground of the video, and convert it into a binary image.
		mog( capture , foreground , 0.01 ) ;
		threshold( foreground , foreground , 64 , 255 , THRESH_BINARY ) ;

		dilate( foreground , foreground , Mat( ) , Point( -1 , -1 ) ) ;				//Using the filter to dilate the foreground.

		//Apply masking to get the colored foreground.
		for( int i = 0 ; i < capture.rows ; i ++ )
		{
			for( int j = 0 ; j < capture.cols ; j ++ )
			{
				//If the gray level value of foreground image is black.
				if( foreground.at<uchar>( i , j ) == 0 )
				{
					//Then set color of the corresponding coordinate of capture image as black.
					capture.at<Vec3b>( i , j )[ 0 ] = 0 ;
					capture.at<Vec3b>( i , j )[ 1 ] = 0 ;
					capture.at<Vec3b>( i , j )[ 2 ] = 0 ;
				}
			}
		}

		//Write the image file on disk, and set its file name.
		if( nowFrame >= 256 && nowFrame <= 270 )
		{

			fileName = itoa( nowFrame , buffer , 10 ) ;
			fileName += ".jpg" ;

			imwrite( fileName , capture ) ;
		}

		imshow( "Foreground" , capture ) ;							//Show the image after processing.
	}

	cap.release( ) ;												//Release the video capture.

	return 0 ;
}

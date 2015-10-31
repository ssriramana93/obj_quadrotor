#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "keypoint.h"
#include "SIFT.h"
#define N 4
//#define w 320
//#define h 240
class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w , int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		ofVideoGrabber vid;
		ofxCvColorImage color,sec;
		ofxCvGrayscaleImage gr;
		IplImage *img,*pix2,*pix3;
		int refkey[10][2];
		unsigned int numrefkey;
		int h,w;
		int t;
	double	hist1[256 * 3],hist2[256 * 3],refhist[256*3];
		double obj[8];
		 double updt[N][9];

		unsigned char *pix1,pix[80 * 20];
		

};

#include "testApp.h"

float compkey(int refkey[][2],unsigned int numrefkey,int key[][2],unsigned int numkey);
float comphist(double *hist1,double *hist2);
void crhist(unsigned char *img,double *hist,int w,int h,double obj[4]);
void resample(unsigned char *img,double *refhist, int refkey[][2],unsigned int *numrefkey,double updt[][9],double *pconf,int w,int h,int t);
void evolve(double updt[][9]);
void msrmtupdate(unsigned char *img,double updt[][9], int refkey[][2],unsigned int *numrefkey,double *refhist,int w,int h);
void exobj(double updt[][9],double *obj);

void evolve(double updt[][9])
{
	srand(time(NULL));
	for(int i=0;i<N;i++)
	{ 
		updt[i][4]=(rand()%30-20)*0;
		updt[i][5]=(rand()%30-20)*0;
		updt[i][6]=(rand()%10-5);
		updt[i][7]=(rand()%10-5);
		//cout<<updt[i][4]<<'\t'<<updt[i][5]<<'\n';
		updt[i][0]+=updt[i][4];
		updt[i][1]+=updt[i][5];
		//updt[i][2]+=updt[i][6];
		//updt[i][3]+=updt[i][7];

			if((updt[i][0]<0 || updt[i][0]>240))
				updt[i][0]-=updt[i][4];
			if((updt[i][1]<0 || updt[i][1]>320))
				updt[i][1]-=updt[i][5];
			if((updt[i][2]<10 ))
				updt[i][2]-=updt[i][6];
			if(updt[i][3]<10)
				updt[i][3]-=updt[i][7];

				
	}
}
void msrmtupdate(unsigned char *img,double updt[][9],int refkey[][2],unsigned int *numrefkey,double *refhist,int w,int h)
{
	double tot=0,obj1[8]={0,0,0,0,0,0,0,0},obj2[8]={0,0,0,0,0,0,0,0};
	double hist[257*30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},ptr[4]={0,0,0,0};
	unsigned char pix[80 * 60*3]={0,0,0};
	int key[10][2];
	//cout<<"a="<<updt[4][1]<<'\n';ned 
	exobj(updt,obj1);
	
	for(int i=0;i<N;i++)
	{ 
		for(int j=0;j<4;j++)
		{
		ptr[j]=updt[i][j];
		//cout<<ptr[j]<<'\n';
		}
		int x=ptr[0],y=ptr[1],wt=ptr[2],ht=ptr[3],p=0;
	for(long int i=w*(y-ht/2);i<w*h && i<w*(y+ht/2);i++)
	{

		
		if(i%w > x-wt/2 && i%w < x+wt/2)
		{
		pix[3*p] = img[3*i];
		pix[3*p+1]=img[3*i+1];
		pix[3*p+2]=img[3*i+2];
		//img[3*i]=0;img[3*i+1]=255;img[3*i+2]=0;	
		p++;
		}
		
	}
	ofxCvColorImage sec;
	
	sec.setFromPixels(pix,80,60);
	/*IplImage *im;
	im=sec.getCvImage();*/
	SIFT *sift=new SIFT((sec.getCvImage()),4,2);
	sift->DoSift();
	sift->retkptarray(key);
	sift->ShowKeypoints();
unsigned int numkey=sift->m_numKeypoints;
	cout<<"keydist="<<compkey(refkey,*numrefkey,key,numkey)<<"\n";
	//    crhist(img,hist,w,h,ptr);
		//cout<<"N="<<i<<'\n';

//		updt[i][8]=.1*(double)exp(-20*pow(comphist(hist,refhist),2));
		//cout<<updt[i][8]<<'\t';
		tot+=updt[i][8];
	}
	//cout<<tot;
	for(int i=0;i<N;i++)

	{
		updt[i][8]/=(double)tot;
	//cout<<updt[i][8]<<'\t';
	}
	cout<<'\n';
	cout<<"sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss"<<"\n";
	/*for(int i=0;i<N;i++)
	{
		for(int j=i+1;j<N;j++)
		{
			if(updt[j][8]<updt[i][8])
			{
				for(int k=0;k<9;k++)
				{
					double temp=updt[i][k];
					updt[i][k]=updt[j][k];
					updt[j][k]=temp;
				}
			}
		}
	}*/
	
	
}
void exobj(double updt[][9],double *obj)
{
for(int i=0;i<8;i++)
{
	obj[i]=0;
for(int j=0;j<N;j++)
{
	obj[i]+=(double)updt[j][8]*updt[j][i];
	//cout<<updt[j][8]*updt[j][i]<<"\t";
}
}

}
int sign(double x)
	{
		if(x>0)
			return 1;
		else if(x==0)
			return 0;
		else
			return -1;
	}
void init(unsigned char *img,double *refhist,int refkey[][2],unsigned int *numrefkey,double updt[][9],int w,int h)
{
	double obj[8]={0};
	unsigned char pix[60*80*3]={0,0};
	
	obj[0]=w/2;obj[1]=h/2;obj[2]=80;obj[3]=60;obj[4]=obj[5]=1;obj[6]=obj[7]=0;
	int x=obj[0],y=obj[1],wt=obj[2],ht=obj[3];
	int p=0,l=0;
	for(long int i=w*(y-ht/2);i<w*h && i<w*(y+ht/2);i++)
	{
		if(i%w==x-wt/2+5)
			l++;
		
		if(i%w > x-wt/2 && i%w < x+wt/2)
		{
		pix[3*p] = img[3*i];
		pix[3*p+1]=img[3*i+1];
		pix[3*p+2]=img[3*i+2];
		p++;	
		img[3*i]=0;img[3*i+1]=255;img[3*i+2]=0;
		}
	
	}
	cout<<l<<'\t';	
	ofxCvColorImage sec;
	
	sec.setFromPixels(pix,80,60);
	IplImage *im;
	im=sec.getCvImage();
	SIFT *sift=new SIFT(im,4,2);
	sift->DoSift();
	sift->ShowKeypoints();
	sift->retkptarray(refkey);
	*numrefkey=sift->m_numKeypoints;
	
	for(int i=0;i<N;i++)
	{
		updt[i][0]=obj[0]+(rand()%30-30);
		updt[i][1]=obj[1]+(rand()%30-30);
		updt[i][2]=obj[2];
		updt[i][3]=obj[3];
		updt[i][4]=obj[4]+(rand()%2-2);
		updt[i][5]=obj[5]+(rand()%2-2);
		updt[i][6]=obj[6];
		updt[i][7]=obj[7];
		updt[i][8]=(double)1/100;
	img[3*(int)(w*updt[i][1]+updt[i][0])]=0;img[3*(int)(w*updt[i][1]+updt[i][0])+1]=255;img[3*(int)(w*updt[i][1]+updt[i][0])+1]=0;
	}
}
	
void resample(unsigned char *img,double *refhist,int refkey[][2],unsigned int *numrefkey,double updt[][9],double *pconf,int w,int h,int t)
{	
	double obj[8]={0},tot=0;
	double temp[N][10];
	cout<<updt[99][8]<<"\t"<<updt[0][8]<<"\n";
	temp[0][9]=0;
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<9;j++)
		{
			temp[i][j]=updt[i][j];
		}
		if (i>0)
		temp[i][9]=temp[i-1][9]+updt[i][8];
		tot+=updt[i][8];
	}
	cout<<"\ntot="<<tot<<"\n";
	for(int i=0;i<N;i++)
	{
		double tmp=rand()%100+1;
		tmp=tmp/100;
		updt[i][1]=temp[99][1];
		updt[i][0]=temp[99][0];
		updt[i][4]=temp[99][4];
		updt[i][5]=temp[99][5];
		updt[i][8]=1/N;
		int rt=0,lt=N,key=(rt+lt)/2;
		while(rt!=lt+1 && lt!=rt+1)
		{
		key=(rt+lt)/2;
		if(temp[key][9]==tmp)
		{
			break;
		}
		else if(temp[key][9]>tmp)
		{
			lt=key;
		}
		else if(temp[key][9]<tmp)
		{
			rt=key;
		}
		}
		cout<<key<<"\t";
		updt[i][1]=temp[key][1];
		updt[i][0]=temp[key][0];
		updt[i][3]=temp[key][3];
		updt[i][2]=temp[key][2];
		updt[i][4]=temp[key][4];
		updt[i][5]=temp[key][5];
		updt[i][6]=temp[key][6];
		updt[i][7]=temp[key][7];
		updt[i][8]=(double)1/N;
	
		
	
		img[3*(int)(w*updt[i][1]+updt[i][0])]=0;img[3*(int)(w*updt[i][1]+updt[i][0])+1]=255;img[3*(int)(w*updt[i][1]+updt[i][0])+1]=0;
	}
}
	
    
float compkey(int refkey[][2],unsigned int numrefkey,int key[][2],unsigned int numkey)
{
	float dist=0;
	for(int i=0;i<numkey;i++)
	{
		float temp=10000;
		for(int j=0;j<numrefkey;j++)
		{
			float temp1=sqrt(pow((float)(key[i][0]-refkey[j][0]),2)+pow((float)(key[i][1]-refkey[j][1]),2));
			temp=(temp<temp1)?temp:temp1;
		}
		dist+=temp;
	}
	dist/=numkey;
	return dist;
}




	 
	
	
	


void crhist(unsigned char *img,double *hist,int w,int h,double obj[4])
{
	long int i=0;
	int x,y,wt,ht,tot=0;
	//unsigned char pix[220*82];
	 x=obj[0];y=obj[1];wt=obj[2];ht=obj[3]; 
	double mr=0,mg=0,mb=0;
	for(i=0;i<=(256*3);i++)
	{ 
		hist[i]=0;
	}
	//int p=0;

	for(i=w*(y-ht/2);i<w*h && i<w*(y+ht/2); i++)
	{
		
		if(i%w > x-wt/2 && i%w < x+wt/2)
		{
	//	pix[p++] = img[i];
			tot++;
		int	r=img[3*i],g=img[3*i+1],b=img[3*i+2];
		hist[r]++;
		hist[255+g]++;
		hist[255*2+b]++;
		if(i<=w*((y-ht)+2) || i>=w*((y+ht)-2)  )
		{
		//img[3*i]=0;img[3*i+1]=255;img[3*i+2]=255;
		}
		}
		
	}

	
	


	mr=mg=mb=tot;
	for(i=0;i<255;i++)
	{
		hist[i]/=mr;
		hist[255+i]/=mg;
		hist[255*2+i]/=mb;
	}
	
	
	
}
void rep(unsigned char *img,double *obj,int w,int h)
{
		int x,y,wt,ht,tot=0;
	 x=obj[0];y=obj[1];wt=obj[2];ht=obj[3];
	 
		for(int i=w*(y-ht/2);i<w*h && i<w*(y+ht/2);i++)
		{

			if(i%w > x-wt/2 && i%w < x+wt/2)
		{
			
			if(i>=w*((y-ht/2)+2) || i<=w*((y+ht/2)-2)  )
		{

	
		img[3*i]=0;img[3*i+1]=255;img[3*i+2]=255;
		}
		}
		}
}
float comphist(double *hist1,double *hist2)
{
	double dist[3]={0.0,0.0,0.0};
	for(int i=0;i<255;i++)
	{

		dist[0]+=sqrt(hist1[i]*hist2[i]);
		dist[1]+=sqrt(hist1[255+i]*hist2[255+i]);
		dist[2]+=sqrt(hist1[255*2+i]*hist2[255*2+i]);
		
	}
	dist[0]=sqrt(1-dist[0]);
	dist[1]=sqrt(1-dist[1]);
	dist[2]=sqrt(1-dist[2]);
	
	return((dist[0]+dist[1]+dist[2])/3);
	
}


	
//--------------------------------------------------------------
void testApp::setup(){
	w=320;h=240;t=0;
	vid.setVerbose(true);
	vid.listDevices();
	vid.setDesiredFrameRate(100);
        vid.initGrabber(w,h);
		color.allocate(w,h);
		gr.allocate(w,h);
}

//--------------------------------------------------------------
void testApp::update(){
	
//	obj[0]=100;obj[1]=h/2;obj[2]=20;obj[3]=20;
	vid.grabFrame();
	
	if(vid.isFrameNew())
	{
	color.setFromPixels(vid.getPixels(), w,h);
		
	if(t==0)
	{
    //pix1=color.getPixels();
	cv::cvtColor(color, pix1, CV_BGR2Lab);
	
	obj[0]=w/2;obj[1]=h/2;obj[2]=80;obj[3]=20;obj[4]=obj[5]=1;obj[6]=obj[7]=0;
	crhist(pix1,refhist,w,h,obj);
	pix1[3*(int)(w*obj[1]+obj[0])]=255;pix1[3*(int)(w*obj[1]+obj[0])+1]=0;pix1[3*(int)(w*obj[1]+obj[0])+2]=255;
	init(pix1,refhist,refkey,&numrefkey,updt,w,h);
	color.setFromPixels(pix1,w,h);

	}
	if(t>0)
	{
	cout<<'l';
	for(int i=0;i<9;i++)
	cout<<updt[2][i]<<'\t';
	cout<<endl;
	evolve(updt);
	 msrmtupdate(pix1,updt,refkey,&numrefkey,refhist,w,h);
	 
	
	resample(pix1,refhist,refkey,&numrefkey,updt,0,w,h,t);
	exobj(updt,obj);
	rep(pix1,obj,w,h);

	
	}

	
	color.setFromPixels(pix1,w,h);
	}
}
//--------------------------------------------------------------
void testApp::draw(){
	
	
	{
	color.draw(w,h);
	 // set our color to light blue
ofFill();

	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	t++;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}
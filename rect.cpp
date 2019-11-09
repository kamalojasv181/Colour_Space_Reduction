#include<iostream>
#include<queue>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#define mp make_pair
#define pb pushback

typedef long long int ll;

Mat img;
ll visited[720][1280];
ll rows,cols;
ll ccind = 0;
int thres=20;
double meanarr[1000000][3];

bool isvalid(int i,int j)
{
	return i>=0 && j>=0 && i<rows && j<cols;
}

bool addable(int x,int y, double * mean, ll cnt)
{
	if(visited[x][y]!=0)	return 0;
	for(int i=0;i<3;i++)
	{
		if(abs(mean[i]-img.at<Vec3b>(x,y)[i])>thres)	return 0;
	}
	return 1;
}

void bfs(ll x,ll y)
{
	queue<pair<ll,ll> > qu;
	qu.push(mp(x,y));
	ccind++;
	visited[x][y] = ccind;
	double mean[3];
	for(int i=0;i<3;i++)
	{
		mean[i] = img.at<Vec3b>(x,y)[i];
	}
	ll cnt=1;

	while(!qu.empty())
	{
		//cout<<"cnt "<<cnt<<endl;
		pair<ll,ll> pa = qu.front();
		x = pa.first, y = pa.second;
		for(int i=-1;i<2;i++)
		{
			for(int j=-1;j<2;j++)
			{
				if((i==0 && j==0) || !isvalid(x+i,y+j)) continue;
				if(addable(x+i,y+j,mean,cnt))
				{
					visited[x+i][y+j] = ccind;
					qu.push(mp(x+i,y+j));
					for(int k=0;k<3;k++)	
						mean[k] = (mean[k]*cnt+img.at<Vec3b>(x+i,y+j)[k])/(cnt+1);
					cnt++;
				}
			}
		}
		qu.pop();
	}
	//cout<<"cnt "<<cnt<<endl;
	for(int i=0;i<3;i++)	
		meanarr[ccind][i] = mean[i];
}

int main()
{
	VideoCapture cap(0);
	namedWindow("window",WINDOW_NORMAL);
	createTrackbar("thres","window",&thres,100);
	//for(int i=0;i<1500;i++)
	//	cap>>img;
	while(1)
	{
		cap>>img;
		GaussianBlur(img,img,Size(3,3),0);
		Size size(360,640);
		//resize(img,img,size);
		rows = img.rows, cols = img.cols;
		for(int i=0;i<rows;i++)
		{
			for(int j=0;j<cols;j++)
			{
				visited[i][j]=0;
			}
		}
		ccind = 0;
		
		for(int i=0;i<rows;i++)
		{
			for(int j=0;j<cols;j++)
			{
				if(visited[i][j]==0)
				{
					//cout<<ccind<<"\n";
					bfs(i,j);
				}
			}
		}

		//cout<<"ccind "<<ccind<<endl;
		Mat img2 = img.clone();
		for(int i=0;i<rows;i++)
		{
			for(int j=0;j<cols;j++)
			{
				for(int k=0;k<3;k++)
				{
					img2.at<Vec3b>(i,j)[k] = meanarr[visited[i][j]][k];
				}
			}
		}
		imshow("window",img2);
		waitKey(10);
	}
}

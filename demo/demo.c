#include<stdio.h>

double process(double n[])
{
	double ans=0;
	for(int i=0;i<10;i++)
	{
		
		ans=n[i%3];
		printf("+++++++++++++++%d+++++++++\n",i%3);
		printf("%lf\n",ans);
	}
	
	return ans;
}


int main(){
	double n[3];
	freopen("in.txt","r",stdin); 
	freopen("fffff.txt","w",stdout);
  	scanf("%lf %lf %lf",&n[0],&n[1],&n[2]);
	//printf("%f %f %f",n[0],n[1],n[2]);
	double ans=process(n);
	
	printf("n+m+k:%lf\n",ans); 
	ans=process(n);
        ans=process(n);
        printf("n+m+k:%lf\n",ans);
	fclose(stdin);
	fclose(stdout);
	return 0;
}


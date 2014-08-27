#include<iostream>
#include<string>
#include<string.h>
#include<cmath>
#include<stdio.h>
using namespace std;
int c[26];
char b[32];
int r1,r2;
int bara,barb; 
float dice[13];




typedef struct node
{
	int state[32];
	float h;
	struct node *child;
	struct node *bro;
	int m11,m12,m21,m22;
	
}node;


node * root;


int print_moves(node *temp)
{
	if(temp==NULL)
	{
		cout<<"pass"<<endl;
		return 0;
	}
	
	
	if(temp->m11!=temp->m12)
	{
	if(temp->m11==0)
		cout<<'Z'<<" ";
	else if(temp->m11!=-1)
		cout<<temp->m11<<" ";
	else
		cout<<"pass";


	if(temp->m12==25 && temp->m11!=-1)
		cout<<'O';
	else if(temp->m11!=-1) 
		cout<<temp->m12;
	}
	else
		cout<<"pass";
	cout<<endl;
	
	if(temp->m21!=temp->m22)
	{
	if(temp->m21==0)
		cout<<'Z'<<" ";
	else if(temp->m21!=-1)
		cout<<temp->m21<<" ";
	else
		cout<<"pass"<<endl;


	if(temp->m22==25 && temp->m21!=-1)
		cout<<'O'<<endl;
	else if(temp->m21!=-1)
		cout<<temp->m22<<endl;
	}

	else
		cout<<"pass"<<endl;
}


float compute_kill_chance(int i,int a[])
{
	float chance=0.0;
	int j;
	for(j=24;j>i;j--)
	{
		if(j>i && a[j]<0 && j-i<=12)
		{
			chance+=dice[j-i];
		}
	}
	if(a[26]>0)
	{
		if(i>=13)
		{
			chance+=dice[25-i];
		}
	}
	return chance;
	
}

int cal_my_pins(int *state)
{
	int pin_count=0,i;
	for(i=0;i<=24;i++)
	{
		if(state[i]>0)
		{
			pin_count+=state[i]*(25-i);
		}
	}
	return pin_count;
}

float cal_HP(int * state)
{
	float val=0.0;
	int i;
	for(i=0;i<=24;i++)
	{
		if(state[i]>0)
		{
			val+=state[i]*(25-i);	
					
			if(state[i]==1)
			{
				val+=compute_kill_chance(i,state)*(i+25);
			}
		}
		
	}



	val+=state[25]*25;// adding second time for bar which means bar is too bad

	int k1=cal_my_pins(state);
	int inverted_state[30],k;
	int x=state[0];
	for(k=0;k<=25;k++)
	{
		inverted_state[k]=-1*state[25-k];
	}
	inverted_state[0]=state[26];// 26=enemy bar
	inverted_state[26]=x;
	int k2=cal_my_pins(inverted_state);


	if(k1+18<k2)
	{
		val-=state[5]*0;// 6=holding, 3=equal , 1=running
		val-=state[7]*0;
		val-=state[4]*0;
		val-=state[3]*0;
		val-=state[2]*0;
		// k1 is winning
		// running game
	}
	else if(k2+18<k1)
	{
		val-=state[5]*6;// 6=holding, 3=equal , 1=running
		val-=state[7]*3;
		val-=state[4]*2.5;
		val-=state[3]*1.5;
		val-=state[2]*1;
		// k2 is winning
		// holding
	}
	else if(k1+18>k2 && k1<k2)
	{
		val-=state[5]*3;// 6=holding, 3=equal , 1=running
		val-=state[7]*1;
		val-=state[4]*0;
		val-=state[3]*0;
		val-=state[2]*0;
		// slightly winnig
	}
	else if(k2+18>k1 && k2<k1)
	{
		val-=state[5]*4;// 6=holding, 3=equal , 1=running
		val-=state[4]*2;
		val-=state[7]*2;
		val-=state[3]*1.5;
		val-=state[2]*1;
		// slightly loosing
	}
	
//	cout<<val<<endl;
return val;
}




float heuristic(int state[])
{
	// my bar=state[0]
	// enemy bar=state[26]
	float my_hueristic=0,enemy_hueristic=0;
	int i;

	my_hueristic=cal_HP(state);
	//cout<<my_hueristic<<endl;
	//cout<<my_hueristic<<endl;

	int inverted_state[32];
	int x=state[0],k=0;

	
	while(k<=25)
	{
	
		inverted_state[k]=-1*state[25-k];
		k++;
	}
	inverted_state[0]=state[26];// 26=enemy bar
	inverted_state[26]=x;

	
	enemy_hueristic=cal_HP(inverted_state);
	
	return my_hueristic;//-enemy_hueristic;
}




void set()
{
	int i;

	c[26]=barb;
	c[0]=bara;
	int rem=0;
	for(i=0;i<=26;i++)
	{
		root->state[i]=c[i];
		if(c[i]>0)		
		rem+=c[i];	
	}
	c[25]=15-rem;
	root->state[25]=c[25];
	
		dice[1]= 0.16+0.16;
		dice[2]= 0.16+0.16+0.04;
		dice[3]= 0.16+0.16+0.08;
		dice[4]= 0.16+0.16+0.12;
		dice[5]= 0.16+0.16+0.16;
		dice[6]= 0.16+0.16+0.20;
		dice[7]= 0.16;
		dice[8]= 0.15;
		dice[9]= 0.12;
		dice[10]=0.09;
		dice[11]=0.06;
		dice[12]=0.03;
}


int gen_moves_enemy(int state[],int d1,int d2)
{

	//cout<<endl<<" reached "<<endl;	
	int i,j,k;
	//cout<<d1<<" "<<d2<<endl;
	node *dummy, *temp;
	dummy=new node;				
//	dummy->bro->child=dummy;
	int min=1000;			
	temp=dummy;

	for(i=0;i<=25;i++)
	{ int m11=-1,m12=-1;
		int new_state[32];
		for(j=0;j<=26;j++)
			new_state[j]=state[j];
		
		int bearing;
		int off=30;
		for(j=0;j<=18;j++)
		{
			if(new_state[j]>0)
			{
				off=24;
				//cout<<"no bearing3"<<endl;
				break;
			}
		}	

		
		if(new_state[i]>0)
		{
			if(new_state[i+d1]>=-1 && (i+d1<=24 || off==30))
			{
				new_state[i]--;
				m11=i;
				
				if(new_state[i+d1]==-1)
				{
					if(i+d1 <= 24)
					{
						new_state[i+d1]=1;
						m12=i+d1;
					}
					else if(off==30 && i+d1>24)
					{
						m12=25;
						new_state[25]++;
					}
					new_state[26]++;
				}
				else
				{
					if(i+d1 <= 24)
					{
						m12=i+d1;				
						new_state[i+d1]++;
					}
					else if(off==30 && i+d1>24)
					{
						m12=25;
						new_state[25]++;
					}
				}
			}
			

			for(j=0;j<25;j++)
			{
			int l;
			int m21=-1,m22=-1;
			for(l=0;l<=18;l++)
			{
				if(new_state[l]>0)
				{
					off=24;
					//cout<<"no bearing4"<<" ";
					break;
				}
			}	
				m21=j;
			   	if(new_state[j]>0)
			   	{
					if(new_state[j+d2]>=-1 && j+d2<=24 && i!=j)
					{	
						new_state[j]--;
						if(new_state[j+d2]==-1)
						{
							if(j+d2 <= 24)
							{
								m22=j+d2;
								new_state[j+d2]=1;
							}
							else if(off==30)
							{
								m22=25;
								new_state[25]++;
							}
							
							new_state[26]++;
						}
						else
						{
							if(j+d2 <= 24)
							{
								m22=j+d2;
								new_state[j+d2]++;
							}
							else if(off==30)
							{						
								new_state[25]++;
								m22=25;
							}
						}
					
					dummy->bro=new node;
					for(k=0;k<=25;k++)
					{
						dummy->bro->state[k]=new_state[k];
					}
					dummy->bro->m11=m11;
					dummy->bro->m12=m12;
					dummy->bro->m21=m21;
					dummy->bro->m22=m22;
					dummy->bro->bro=NULL;
					dummy->bro->child=NULL;
					//cout<<"reached"<<endl;

					dummy->bro->h=heuristic(dummy->bro->state);
					//cout<<"reached here"<<endl;
					//cout<<"sup1"<<dummy->bro->h<<endl;
					dummy=dummy->bro;
					if(m22!=-1)
					{
						new_state[m21]++;
						new_state[m22]--;
					}
					}
			   	}
			}
		}
	}
	
	node *temp2=temp;
	
	if(temp2->bro!=NULL)
	{
		min=temp2->bro->h;
	}
	while(temp2->bro!=NULL)
	{
			if(min>=temp2->bro->h)
				min=temp2->bro->h;
			temp2=temp2->bro;

	}
	
	
	return min;

}

float enemy(int *dummy)
{
	int i,j,k;
	float expectimax=0.0;
	for(i=1;i<=6;i++)
	{
		for(j=1;j<=6;j++)
		{
			node * dummy1=new node;
			int x=dummy[0];
			node *temp=dummy1;
						
			
			for(k=0;k<=25;k++)
			{
				temp->state[k]=-1*dummy[25-k];
			}
			temp->state[0]=dummy[26];// 26=enemy bar
			temp->state[26]=x;
		
			dummy1->h=gen_moves_enemy(temp->state,i,j);
			
			expectimax+=dummy1->h;

		}
	}

return expectimax/36;
	
}





node *gen_moves(int state[],int d1,int d2)
{
	int i,j,k;
	node *dummy, *temp;
	dummy=new node;				
	dummy->bro=NULL;		
	root->child=dummy;
			
	temp=dummy;
	
	

	for(i=0;i<25;i++)
	{
		int new_state[32];
		int m11=-1,m12=-1;
		for(j=0;j<=26;j++)
		{
			new_state[j]=state[j];
		}
		
		

		int bearing;
		int off=30;
		for(j=0;j<=18;j++)
		{
			if(new_state[j]>0)
			{
				
				off=24;
				break;
			}
		}

		if(new_state[i]>0)
		{
			if(new_state[i+d1]>=-1 && (i+d1<=24 || off==30) )
			{
				new_state[i]--;
				m11=i;
				
				if(new_state[i+d1]==-1)
				{
					if(i+d1 <= 24)
					{
						new_state[i+d1]=1;
						m12=i+d1;
					}
					else if(off==30)
					{
						m12=25;
						new_state[25]++;
					}
					new_state[26]++;
				}
				else
				{
					
					if(i+d1 <= 24)
					{
						m12=i+d1;				
						new_state[i+d1]++;
					}
					else
					{
						
						m12=25;
						new_state[25]++;
					}
				}
			}
			int new_bar=new_state[0];
			if(new_state[0] > 0 && new_state[0]==state[0])
				m11=m12=-1;
			
			else	
			for(j=0;j<25;j++)
			{
									
				int l;
				int m21=-1,m22=-1;
				for(l=0;l<=18;l++)
				{
					if(new_state[l]>0)
					{
						off=24;
						break;
					}
				}	
			   	if(new_state[j]>0)
			   	{
					
					//cout<<"j="<<j<<endl;
					if(new_state[j+d2]>=-1 && (j+d2<=24 || off==30))
					{	
						
						m21=j;
						new_state[j]--;
						if(new_state[j+d2]==-1)
						{
							if(j+d2 <= 24)
							{
								m22=j+d2;
								new_state[j+d2]=1;
							}
							else if(off==30)
							{
								m22=25;
								new_state[25]++;
							}
							
							new_state[26]++;
						}
						else
						{
							if(j+d2 <= 24)
							{
								m22=j+d2;
								new_state[j+d2]++;
							}
							else if(off==30)
							{						
								new_state[25]++;
								m22=25;
							}
						}
					


					if(new_state[0]>0 && new_state[0]==new_bar)
					{
						//cout<<new_state[0]<<" "<<new_bar<<" "<<j<<endl;						
						m21=m22=-1;
						dummy->bro=new node;
						dummy->bro->m11=m11;
						dummy->bro->m12=m12;
						dummy->bro->m21=-1;							
						dummy->bro->m22=-1;
						dummy->bro->h=enemy(dummy->bro->state);
						dummy->bro->h-=0.5*heuristic(dummy->bro->state);
						if(dummy->bro->m11==-1 || dummy->bro->m12==-1 || dummy->bro->m21==-1 || dummy->bro->m22==-1)
								dummy->bro->h-=2000;
						if(m22-m21+m12-m11 < d1+d2 && off==30)
								dummy->bro->h-=100*(d1+d2-(m22-m21+m12-m11));				

						break;
					}
					dummy->bro=new node;
					for(k=0;k<=26;k++)
					{
						dummy->bro->state[k]=new_state[k];
					}

					dummy->bro->m11=m11;
					dummy->bro->m12=m12;
					
					
					
					dummy->bro->m21=m21;
					dummy->bro->m22=m22;
										
					
					dummy->bro->bro=NULL;
					dummy->bro->child=NULL;

					
											
					dummy->bro->h=enemy(dummy->bro->state);
					dummy->bro->h-=0.5*heuristic(dummy->bro->state);
					if(dummy->bro->m11==-1 || dummy->bro->m12==-1 || dummy->bro->m21==-1 || dummy->bro->m22==-1)
							dummy->bro->h-=2000;
					if(m22-m21+m12-m11 < d1+d2 && off==30)
							dummy->bro->h-=100*(d1+d2-(m22-m21+m12-m11));				

//					if(off==30)
//						dummy->bro->h-=cal_my_pins(dummy->bro->state)*2;
//					cout<<m11<<" "<<m12<<" "<<m21<<" "<<m22<<" "<<dummy->bro->h<<endl;	
					
					//cout<<cal_my_pins(dummy->bro->state)<<endl;				



					dummy=dummy->bro;
					if(m22!=-1)
					{
						new_state[m21]++;
						new_state[m22]--;
					}

					
					}
			   	}
			}
		}
	}

//temp=root->child;
node * temp2=temp->bro;


while(temp->bro!=NULL)
{
//cout<<temp->bro->m11<<" "<<temp->bro->m12<<" "<<temp->bro->m21<<" "<<temp->bro->m22<<" "<<temp->bro->h<<endl;	
	if(temp->bro->h >= temp2->h)
		temp2=temp->bro;
	
//	cout<<temp->bro->h<<endl;
	temp=temp->bro;
}

return temp2;

}











int main()
{
int i;
	root=new node;
	for(i=1;i<=24;i++)
	{
	cin>>c[i];
	}
	
	char ch='\0';
	ch=getchar();	
	ch=getchar();
	i=0;
	while(ch!='\n')
	{
		b[i]=ch;	
		i++;
		ch=getchar();
		
	}
	b[i]='\0';

	cin>>r1>>r2;
//	cout<<"die"<<r1<<" "<<r2<<endl;
	
	for(i=0;i<strlen(b);i++)
	{
		if(b[i]=='a')
			bara++;

		if(b[i]=='b')
			barb++;
	}
	set();
	
	node * temp2=gen_moves(c,r1,r2);
	
	node * temp1=gen_moves(c,r2,r1);
	
	if(temp1!=NULL && temp2!=NULL)
	{
	if(temp1->h > temp2->h)
		temp2=temp1;
	}

	if(temp2==NULL)
		temp2=temp1;

	

	print_moves(temp2);	

return 0;
}

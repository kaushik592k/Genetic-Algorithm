#include<bits/stdc++.h>
using namespace std;

#define pc 0.8
#define N 9
#define sqrtN 3
#define POP 8000
#define elite 200
#define GEN 2000
int f;
int arr[N][N],helperR[N][N],helperC[N][N],helperS[N][N],grid[N][N];
double pm=0.4;

struct chromosome{
	int ans[N][N];
	int helprow[N][N],helpcol[N][N],helpsubgrid[N][N];
	double fitness;
}c[POP],best,parent[POP],coparent[POP],newgen[POP],ind1,ind2,offspring,arr_restart[POP];  

bool compare(chromosome a, chromosome b)// Will be used to sort according to fitness
{
	return (a.fitness<b.fitness);
}

void calculate_fitness(chromosome& a)
{
	int uniquecol[N],uniquesub[N];
	int i,j;
	a.fitness=1.0;
	for(i=0;i<N;i++)
	  { uniquecol[i]=0;
		for(j=0;j<N;j++)
		  { if(a.helpcol[i][j]!=0)
		      uniquecol[i]+=1;
		  }
		//cout<<"Column "<<i+1<<" : "<<uniquecol[i]<<"\n";
		a.fitness=a.fitness*(9.0/(double)uniquecol[i]);          //Calculating fitness for column repetitions
	  }
	  
	for(i=0;i<N;i++)
	  { uniquesub[i]=0;
		for(j=0;j<N;j++)
		  { if(a.helpsubgrid[i][j]!=0)
		      uniquesub[i]+=1;
		  }
		//cout<<"Subgrid "<<i+1<<" : "<<uniquesub[i]<<"\n";
		a.fitness=a.fitness*(9.0/(double)uniquesub[i]);         //Calculating fitness for sub-grid repetitions
	  }
	
	//return a.fitness;	  
}

void generate_pop()
{
	int i,j,k,l,x,y;
	helperC[N][N]={0};helperS[N][N]={0};
	for(k=0;k<POP;k++)
	  for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		  {
			c[k].helprow[i][j]=helperR[i][j];
			c[k].helpcol[i][j]=0;
			c[k].helpsubgrid[i][j]=0;
		  }
	
	for(k=0;k<POP;k++)
	  {
	  	for(j=0;j<N;j++)
	  	  for(i=0;i<N;i++)
	  	    {
	  	  	  if(arr[i][j]!=0)
	  	  	    { x=arr[i][j];
	  	  	      helperC[j][x-1]=1;
			      c[k].helpcol[j][x-1]+=1;   //Initializing helpcol with givens
			    }
		    } 
	  }
    
    for(k=0;k<POP;k++)
	  {
		for(i=0;i<N;i++)
		  { for(j=0;j<N;j++)
			  {
			    x=arr[i][j];
			    if(x!=0)
			      { y=sqrtN*(i/sqrtN)+(j/sqrtN);
			        helperS[y][x-1]=1;
			        c[k].helpsubgrid[y][x-1]+=1;          //Initializing helpsubgrid with givens
			      }
			  }
		  } 
	  }
	  
	int possible[N][N][N],countposs[N][N];
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { countposs[i][j]=0;                   //Count array stores the number of possible values at a place
	  	  	for(l=0;l<N;l++)
	  	  	  possible[i][j][l]=0;            //Possible array stores the possible values at a place
		  }
	  }
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { if(arr[i][j]==0)
			  {
			    y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    for(l=0;l<N;l++)
			      { if(helperR[i][l]==0 && helperC[j][l]==0 && helperS[y][l]==0)
			          { possible[i][j][l]=1;
			            countposs[i][j]++;
				      }
			      } 
		      }
		  }
	  }
	
	int times=100;
	while(times--)
	{
	
	for(i=0;i<N;i++)
	  { for(j=0;j<N;j++)
	      {
	      	if(countposs[i][j]==1)               //Those places with only 1 possible value will now be filled with that value
	      	  { for(l=0;l<N;l++)
	      	      { if(possible[i][j][l]==1)
	      	          { arr[i][j]=l+1;
	      	            countposs[i][j]=0;
	      	            possible[i][j][l]=0;
	      	          }
				  }
			  }
		  }
	  } 
	  
    for(i=0;i<N;i++)
      for(j=0;j<N;j++)
        {
        	x=arr[i][j];
        	if(x!=0)
        	  helperR[i][x-1]=1;   //Updating all the helpers after the update in arr by filling obvious blanks
		}
    for(j=0;j<N;j++)
	  for(i=0;i<N;i++)
	  	{
	  	  if(arr[i][j]!=0)
	  	  	{ x=arr[i][j];
	  	  	  helperC[j][x-1]=1;   
			}
		}
    for(i=0;i<N;i++)
	  { for(j=0;j<N;j++)
		  {
			x=arr[i][j];
			if(x!=0)
			  { y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    helperS[y][x-1]=1;
			  }
		  }
	  }
    
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { countposs[i][j]=0;                   //Count array will now store no. of possible values at a place after updating arr
	  	  	for(l=0;l<N;l++)
	  	  	  possible[i][j][l]=0;            //Possible array will now store the possible values at a place after updating arr
		  }
	  }
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { if(arr[i][j]==0)
			  {
			    y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    for(l=0;l<N;l++)
			      { if(helperR[i][l]==0 && helperC[j][l]==0 && helperS[y][l]==0)
			          { possible[i][j][l]=1;
			            countposs[i][j]++;
				      }
			      } 
		      }
		  }
	  }
	
	/* we'll update obvious numbers wrt each row */
	
	int cnt;  
	for(int num=1;num<=N;num++)
	  { for(i=0;i<N;i++)
	      { cnt=0;
		    for(j=0;j<N;j++)
	          { if(possible[i][j][num-1]==1)
	              { cnt++;
				  }
			  }
			if(cnt==1)
			  { for(j=0;j<N;j++)
			      if(possible[i][j][num-1]==1)
			        { arr[i][j]=num;
			          countposs[i][j]=0;
	      	          possible[i][j][num-1]=0;
					}
			  }
		  }
	  }
	
	/* Updating helpers and possible array after filling obvious numbers with respect to each row */
	
	for(i=0;i<N;i++)
      for(j=0;j<N;j++)
        {
        	x=arr[i][j];
        	if(x!=0)
        	  helperR[i][x-1]=1;   //Updating all the helpers after the update in arr by filling obvious blanks
		}
    for(j=0;j<N;j++)
	  for(i=0;i<N;i++)
	  	{
	  	  if(arr[i][j]!=0)
	  	  	{ x=arr[i][j];
	  	  	  helperC[j][x-1]=1;   
			}
		}
    for(i=0;i<N;i++)
	  { for(j=0;j<N;j++)
		  {
			x=arr[i][j];
			if(x!=0)
			  { y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    helperS[y][x-1]=1;
			  }
		  }
	  }
    
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { countposs[i][j]=0;                   //Count array will now store no. of possible values at a place after updating arr
	  	  	for(l=0;l<N;l++)
	  	  	  possible[i][j][l]=0;            //Possible array will now store the possible values at a place after updating arr
		  }
	  }
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { if(arr[i][j]==0)
			  {
			    y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    for(l=0;l<N;l++)
			      { if(helperR[i][l]==0 && helperC[j][l]==0 && helperS[y][l]==0)
			          { possible[i][j][l]=1;
			            countposs[i][j]++;
				      }
			      } 
		      }
		  }
	  }
	  
	/* Now we'll update obvious numbers wrt each column */
	
	for(int num=1;num<=N;num++)  
	  { for(j=0;j<N;j++)
		  { cnt=0;
		    for(i=0;i<N;i++)
	          { if(possible[i][j][num-1]==1)
	              { cnt++;
				  }
			  }
			if(cnt==1)
			  { for(i=0;i<N;i++)
			      if(possible[i][j][num-1]==1)
			        { arr[i][j]=num;
			          countposs[i][j]=0;
	      	          possible[i][j][num-1]=0;
					}
			  }
		  }
	  }
	  
	/* Updating helpers and possible array after filling obvious numbers with respect to each column */
	
	for(i=0;i<N;i++)
      for(j=0;j<N;j++)
        {
        	x=arr[i][j];
        	if(x!=0)
        	  helperR[i][x-1]=1;   //Updating all the helpers after the update in arr by filling obvious blanks
		}
    for(j=0;j<N;j++)
	  for(i=0;i<N;i++)
	  	{
	  	  if(arr[i][j]!=0)
	  	  	{ x=arr[i][j];
	  	  	  helperC[j][x-1]=1;   
			}
		}
    for(i=0;i<N;i++)
	  { for(j=0;j<N;j++)
		  {
			x=arr[i][j];
			if(x!=0)
			  { y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    helperS[y][x-1]=1;
			  }
		  }
	  }
    
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { countposs[i][j]=0;                   //Count array will now store no. of possible values at a place after updating arr
	  	  	for(l=0;l<N;l++)
	  	  	  possible[i][j][l]=0;            //Possible array will now store the possible values at a place after updating arr
		  }
	  }
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { if(arr[i][j]==0)
			  {
			    y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    for(l=0;l<N;l++)
			      { if(helperR[i][l]==0 && helperC[j][l]==0 && helperS[y][l]==0)
			          { possible[i][j][l]=1;
			            countposs[i][j]++;
				      }
			      } 
		      }
		  }
	  }
		  
	/* Now we'll update obvious numbers wrt each subgrid */
	
	//int ct[N];
	for(int num=1;num<=N;num++)  
	  { int ct[N]={0};
	    for(i=0;i<N;i++)
	      { for(j=0;j<N;j++)
	          { y=sqrtN*(i/sqrtN)+(j/sqrtN);
	            if(possible[i][j][num-1]==1)
	              ct[y]++;
			  }
		  }
		for(l=0;l<N;l++)
		  { if(ct[l]==1)
		      { for(i=0;i<N;i++)
		          { for(j=0;j<N;j++)
		              { y=sqrtN*(i/sqrtN)+(j/sqrtN);
		                if(y==l)
		                  { if(possible[i][j][num-1]==1)
		                      { arr[i][j]=num;
			                    countposs[i][j]=0;
	      	                    possible[i][j][num-1]=0; 
							  }
						  }
					  }
				  }
			  }
		  }
	  }
	  
	for(i=0;i<N;i++)
      for(j=0;j<N;j++)
        {
        	x=arr[i][j];
        	if(x!=0)
        	  helperR[i][x-1]=1;   //Updating all the helpers after the update in arr by filling obvious blanks
		}
    for(j=0;j<N;j++)
	  for(i=0;i<N;i++)
	  	{
	  	  if(arr[i][j]!=0)
	  	  	{ x=arr[i][j];
	  	  	  helperC[j][x-1]=1;   
			}
		}
    for(i=0;i<N;i++)
	  { for(j=0;j<N;j++)
		  {
			x=arr[i][j];
			if(x!=0)
			  { y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    helperS[y][x-1]=1;
			  }
		  }
	  }
    
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { countposs[i][j]=0;                   //Count array will now store no. of possible values at a place after updating arr
	  	  	for(l=0;l<N;l++)
	  	  	  possible[i][j][l]=0;            //Possible array will now store the possible values at a place after updating arr
		  }
	  }
	for(i=0;i<N;i++)
	  {
	  	for(j=0;j<N;j++)
	  	  { if(arr[i][j]==0)
			  {
			    y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    for(l=0;l<N;l++)
			      { if(helperR[i][l]==0 && helperC[j][l]==0 && helperS[y][l]==0)
			          { possible[i][j][l]=1;
			            countposs[i][j]++;
				      }
			      } 
		      }
		  }
	  }
	  
    }
    
    cout<<"\nAfter filling predetermined spaces:\n";
    for(i=0;i<N;i++)
      { for(j=0;j<N;j++)
          { 
		    cout<<arr[i][j]<<" ";
		    if(j>0 && j%sqrtN==sqrtN-1)
		      cout<<"  ";
          }
        cout<<"\n";
        if(i>0 && i%sqrtN==sqrtN-1)
          cout<<"\n";
	  }
	//cout<<"\n";
	  
	for(k=0;k<POP;k++)
	  for(i=0;i<N;i++)
	    for(j=0;j<N;j++)
		  { c[k].helprow[i][j]=helperR[i][j];
		    c[k].helpcol[i][j]=helperC[i][j];
			c[k].helpsubgrid[i][j]=helperS[i][j];	  
		  }
	
	/*Now we will initialize the population using the possible values at a place*/

    for(k=0;k<POP;k++)                       //Initialization of Population
	  {
		for(i=0;i<N;i++)
		  {
		  	for(j=0;j<N;j++)
		  	  {
		  	  	if(arr[i][j]!=0)
		  	  	  c[k].ans[i][j]=arr[i][j];
		  	  	else
		  	  	  {
		  	  	  	x=1+rand()%N;
		  	  	  	if(c[k].helprow[i][x-1]==0 && possible[i][j][x-1]==1)
		  	  	  	  {
						c[k].ans[i][j]=x;
		  	  	  	  	c[k].helprow[i][x-1]+=1;
		  	  	  	  }
		  	  	  	    
		  	  	  	else if(c[k].helprow[i][x-1]!=0 || possible[i][j][x-1]==0)
		  	  	  	  {
		  	  	  	  	x=1+rand()%N;
		  	  	  	    while(1)                        //Ensuring that helprow array for any chromosome is an all 1s array
		  	  	  	      {
		  	  	  	      	//x=1+rand()%9;
		  	  	  	      	if(c[k].helprow[i][x-1]==0) //&& possible[i][j][x-1]==1)
		  	  	  	  	      {
						        c[k].ans[i][j]=x;
		  	  	  	  	        c[k].helprow[i][x-1]+=1;
		  	  	  	  	        break;
		  	  	  	          }
		  	  	  	        x=x%N + 1;
						  }
					  }
				  }
			  }
		  }
	  }
	
	for(k=0;k<POP;k++)
	  {
		for(j=0;j<N;j++)
		  {
		  	for(i=0;i<N;i++)
		  	  {
		  	  	x=c[k].ans[i][j];
		  	  	c[k].helpcol[j][x-1]+=1;            //Updating helpcol
			  }
		  }
	  }
	  
	for(k=0;k<POP;k++)
	  {
		for(i=0;i<N;i++)
	      { for(j=0;j<N;j++)
			  {
			    x=c[k].ans[i][j];
			    y=sqrtN*(i/sqrtN)+(j/sqrtN);
			    c[k].helpsubgrid[y][x-1]+=1;          //Updating helpsubgrid
			  }
		  } 
	  }
	
	for(k=0;k<POP;k++)
	  {
	  	c[k].fitness=1.0;
	  	calculate_fitness(c[k]);
	  }	
	  
}  
//End of generate_pop() function

chromosome tournament(chromosome p[POP])
{
	int i,j,k,M=10;     //M is for this tournament being an M-way tournament
	chromosome temp[M];
	for(k=0;k<M;k++)
	  { int r=rand()%POP;
	    temp[k]=p[r];
	  }
	sort(temp,temp+M,compare);
	return temp[0];
}

void mutate(chromosome& offspring,double pm)
{
	double r= (double)rand()/((double)RAND_MAX + 1.0);
	if(r<pm)
	  {
	  	int rnd=rand()%N;
		int r1=rand()%N;
		int r2=rand()%N;
		if(arr[rnd][r1]==0 && arr[rnd][r2]==0)
		  { int temp = offspring.ans[rnd][r1];
			offspring.ans[rnd][r1] = offspring.ans[rnd][r2];   //Swap mutation
			offspring.ans[rnd][r2]=temp;
		  }
	  }
}

chromosome crossover(chromosome a, chromosome b,double pm)
{
	chromosome offspring;
	int i,j,r,x,y;
	r=rand()%N;
	for(i=0;i<r;i++)
	  for(j=0;j<N;j++)
	    offspring.ans[i][j]=a.ans[i][j];
	for(i=r;i<N;i++)
	  for(j=0;j<N;j++)
	    offspring.ans[i][j]=b.ans[i][j];
	    
    //mutate(offspring);                       //Mutation
	
	for(i=0;i<N;i++)
	  { for(j=0;j<N;j++)
		  { offspring.helprow[i][j]=1;
		    offspring.helpcol[i][j]=0;
		    offspring.helpsubgrid[i][j]=0;
	      }
	  }
	
	for(j=0;j<N;j++)
	  {
		for(i=0;i<N;i++)
	      {
		  	x=offspring.ans[i][j];
		  	offspring.helpcol[j][x-1]+=1;
		  }
	  }
	  
	for(i=0;i<N;i++)
	  { for(j=0;j<N;j++)
		  {
			x=offspring.ans[i][j];
			y=sqrtN*(i/sqrtN)+(j/sqrtN);
			offspring.helpsubgrid[y][x-1]+=1;
		  }
	  }
	offspring.fitness=1.0;
	calculate_fitness(offspring);
	
	chromosome mu=offspring;
	mutate(mu,pm);
	for(i=0;i<N;i++)
	  { for(j=0;j<N;j++)
		  { mu.helprow[i][j]=1;
		    mu.helpcol[i][j]=0;
		    mu.helpsubgrid[i][j]=0;
	      }
	  }
	
	for(j=0;j<N;j++)
	  {
		for(i=0;i<N;i++)
	      {
		  	x=mu.ans[i][j];
		  	mu.helpcol[j][x-1]+=1;
		  }
	  }
	  
	for(i=0;i<N;i++)
	  { for(j=0;j<N;j++)
		  {
			x=mu.ans[i][j];
			y=sqrtN*(i/sqrtN)+(j/sqrtN);
			mu.helpsubgrid[y][x-1]+=1;
		  }
	  }
	mu.fitness=1.0;
	calculate_fitness(mu);
	
	if(mu.fitness <= offspring.fitness)
	  return mu;
	else
	  return offspring;
}
void restart()
{	    
	    /*Till now population has been initialized and fitness of the chromosomes of the initial population has been calculated*/
	    int i,j,k,x,y;
	    sort(c,c+POP,compare);
		for(i=0;i<POP;i++)
		  { 
		  	parent[i]=c[i];
		  }
		best=c[0];
		
		cout<<"Best Initial fitness: "<<best.fitness<<"\n";
		cout<<"Best Initial Chromosome for check:\n";
		for(i=0;i<N;i++)
		  { for(j=0;j<N;j++)
		      { cout<<best.ans[i][j]<<" ";
		        if(j>0 && j%sqrtN==sqrtN-1)
		          cout<<"  ";
		      }
		    cout<<"\n";
		    if(i>0 && i%sqrtN==sqrtN-1)
		      cout<<"\n";
		  }
		cout<<"\n";
		
		int gen=0;
		//while(best.fitness>1.0)
		while(gen<=GEN && best.fitness>1.0)
		  {
		  	for(k=0;k<elite;k++)
		  	  { coparent[k]=parent[k];
		  	    newgen[k]=parent[k];
		  	  }
		  	for(k=elite;k<POP;k++)
		  	  {
		  	  	 coparent[k]=tournament(parent);
			  }
			k=elite;
			while(k<POP)
			  {
			  	int r1=rand()%POP;
			  	int r2=rand()%POP;
			  	double r= (double)rand()/((double)RAND_MAX + 1.0);
			  	if(r<pc)
			  	  { 
					newgen[k++]=crossover(coparent[r1],coparent[r2],pm);    //Mutation function is called inside the crossover function
				  }
			  }
			
			/*Now the new generation population is ready*/
			
			sort(newgen,newgen+POP,compare);
			
			if(newgen[0].fitness<best.fitness)
			  best=newgen[0];
			
			for(k=0;k<POP;k++)
			  parent[k]=newgen[k];
			  
			if(gen%100==1)
			  { 
			  	cout<<"Generation "<<gen<<" : Best fitness = "<<best.fitness<<"\n";
			  }
			for(i=gen;i<gen+4;i++)// To maaintain a population size of 8000 we are taking best 4 fitness from all generations
			arr_restart[i]=newgen[i-gen];
			
			gen++;
		  }
		if(gen>2000&&best.fitness!=1.0)
		{
			f=1;
			cout<<"Restarting the GA"<<endl;
			//generate_POP_restart();
			for(i=0;i<POP;i++)
	    	{
	    		c[i]=arr_restart[i];
			}
			restart();
		}
		cout<<"Fitness of best: "<<best.fitness<<"\n";
		cout<<"No. of generations: "<<gen<<"\n";
		cout<<"\nSolution to Sudoku:\n";
		
		for(i=0;i<N;i++)
		  {
		  	for(j=0;j<N;j++)
		  	  { cout<<best.ans[i][j]<<" ";
		  	    if(j>0 && j%sqrtN==sqrtN-1)
		          cout<<"  ";
		      }
		  	cout<<"\n";
		  	if(i>0 && i%sqrtN==sqrtN-1)
		  	  cout<<"\n";
		  } 
		
		cout<<"\n\n";
}
int main()
{
	srand(time(0));
	int t=1;
	//cout<<"Enter no. of Sudoku puzzles to solve: ";
	//cin>>t;
	while(t--)
	  {
	  	int i,j,k,x,y;
	  	f=0;
	  	for(i=0;i<N;i++)
	  	  for(j=0;j<N;j++)
	  	    helperR[i][j]=0;
	  	cout<<"Enter the soduku puzzle in matrix form (Enter 0 for unknown places) :\n";
	  	for(i=0;i<N;i++)
	  	  {
	  	  	for(j=0;j<N;j++)
	  	  	  {
				 cin>>arr[i][j];
				 x=arr[i][j];
				 if(arr[i][j]!=0)
				   helperR[i][x-1]+=1;   //helper array contains 1 if the value is a given clue, otherwise 0
			  }
		  }
	    generate_pop();
	    restart();
		 
	  }
	  
	return 0;
}

/*
Sample Sudokus for input 
 
0 0 7 0 0 0 9 0 8
0 3 0 1 7 0 0 0 4
0 0 0 0 0 6 0 0 0
6 9 8 7 4 0 3 0 0
0 0 3 0 1 0 4 0 0
0 0 1 0 3 9 7 6 2
0 0 0 4 0 0 0 0 0
9 0 0 0 5 1 0 4 0
4 0 5 0 0 0 1 0 0

0 9 6 0 4 0 0 3 0
0 5 7 8 2 0 0 0 0
1 0 0 9 0 0 5 0 0
0 0 9 0 1 0 0 0 8
5 0 0 0 0 0 0 0 2
4 0 0 0 9 0 6 0 0
0 0 4 0 0 3 0 0 1
0 0 0 0 7 9 2 6 0
0 2 0 0 5 0 9 8 0

0 1 0 2 0 5 0 8 0
0 0 3 0 0 0 4 0 0
2 0 0 0 7 0 0 0 3
0 4 0 6 1 2 0 7 0
0 0 0 0 0 0 0 0 0
0 7 0 3 9 8 0 2 0
4 0 0 0 6 0 0 0 9
0 0 2 0 0 0 8 0 0
0 6 0 9 0 1 0 4 0

0 2 0 6 0 8 0 0 0
5 8 0 0 0 9 7 0 0
0 0 0 0 4 0 0 0 0
3 7 0 0 0 0 5 0 0
6 0 0 0 0 0 0 0 4
0 0 8 0 0 0 0 1 3
0 0 0 0 2 0 0 0 0
0 0 9 8 0 0 0 3 6
0 0 0 3 0 6 0 9 0

0 0 0 0 0 1 4 7 0
0 0 9 0 3 2 0 8 0
6 0 0 0 0 0 0 0 0
0 0 0 3 8 0 0 0 4
0 8 0 6 0 0 0 0 2
0 0 0 2 0 5 3 0 0
0 3 7 0 4 0 6 0 9
2 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 1
*/

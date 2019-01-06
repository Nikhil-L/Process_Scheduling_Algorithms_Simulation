
#include<iostream>
#include<string.h>
#include<fstream>
#define quantum 2
#define maxsize 20
using namespace std;

struct process_details
{
	int processid;
	int bursttime;
	int arrivaltime;
	int priority;
	int completiontime;
	int waittime = 0;
	int turnaroundtime = 0;
};



void destroy_values(struct process_details process[], int n)
{
	int i;
	
	for(int i = 0; i < n; i++)
	{
		process[i].waittime = 0;
		process[i].turnaroundtime = 0;
	}
}


int getdata(struct process_details process[])
{
	int pid;
	int bt;
	int at;
	int pr;
	int i = 0;
	
	ifstream file("process.txt");
	
    if(!file.is_open())
    {
	    cout << "Unable to open the file!!!";
	 	return 0;
	}
	
	while(file >> pid >> bt >> at >> pr)
	{
		process[i].processid = pid;
		process[i].bursttime = bt;
		process[i].arrivaltime = at;
		process[i].priority = pr;
		i++;		
	}
	
	return i;
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void sort(struct process_details process[], int n)
{

	for(int j = 0; j < n- 1; j++)
	{
		for(int i = 0; i < n -j - 1; i++)
		{	
			if(process[i].priority > process[i+1].priority)
			{	
				swap(&process[i].priority, &process[i+1].priority);
				swap(&process[i].bursttime, &process[i+1].bursttime);
				swap(&process[i].arrivaltime, &process[i+1].arrivaltime);
				swap(&process[i].processid, &process[i+1].processid);
			}
		}
	}
}


void waittime_fcfs(struct process_details process[], int n)
{
	int servicetime[n];
	int i;
	process[0].waittime = 0;
	servicetime[0] = 0;
	for(i = 1; i < n; i++)
	{
		servicetime[i] = process[i-1].bursttime + servicetime[i-1];
		process[i].waittime = servicetime[i] - process[i].arrivaltime;
		
		if(process[i].waittime < 0)
		{
			process[i].waittime = 0;
		}
	}	
}

void turnaroundtime(struct process_details process[], int n)
{
	int i;
	
	for(i = 0; i < n; i++)
	{
		process[i].turnaroundtime = process[i].bursttime + process[i].waittime;
	}
}

void avaragetime_fcfs(struct process_details process[], int n, int *averagewaittime, int *averageturnaroundtime)
{
	int totalwaittime = 0;
	int totalturnaroundtime = 0;
	int i;
	
	destroy_values(process, n);
	waittime_fcfs(process, n);
	turnaroundtime(process, n);

	cout << "FIRST COME FIRST SERVE SCHEDULING :\n\n";
	cout << "Process id\t" << "Burst time\t" << "Arrival time\t";
	cout << "Waiting time\t" << "Turnaround time\t" << "Completion time\n";
	
	for(i = 0; i < n; i++)
	{
		totalwaittime = process[i].waittime + totalwaittime;
		totalturnaroundtime = process[i].turnaroundtime + totalturnaroundtime;
		process[i].completiontime = process[i].turnaroundtime + process[i].arrivaltime;
	}		
	
	for(i = 0; i < n; i++)
	{
		cout << process[i].processid << "\t\t" << process[i].bursttime << "\t\t";
		cout << process[i].arrivaltime << "\t\t" << process[i].waittime	<< "\t\t";
		cout << process[i].turnaroundtime << "\t\t" << process[i].completiontime << "\n";		
	}
	
	if(*averagewaittime != 0 && *averageturnaroundtime != 0)
	{
		*averagewaittime = (totalwaittime/n + *averagewaittime)/2;
		*averageturnaroundtime = (totalturnaroundtime/n + *averageturnaroundtime)/2;
	}
	else
	{
		*averagewaittime = totalwaittime/n;
		*averageturnaroundtime = totalturnaroundtime/n;
	}
	
	cout << "The average waiting time is : " << *averagewaittime << "\n";
	cout << "The average turnaround time is : " << *averageturnaroundtime << "\n\n";
}


void waittime_sjf(struct process_details process[], int n)
{
	int rt[n];
	
	for(int i = 0; i < n; i++)
	{
		rt[i] = process[i].bursttime;
	}
	int complete = 0, t = 0, minm = INT8_MAX;
	int shortest = 0, finish_time;
	bool check = false;
	
	while(complete != n)
	{
		for(int j = 0; j < n; j++)
		{
			if((process[j].arrivaltime <= t) && (rt[j] < minm) && (rt[j] > 0))
			{
				minm = rt[j];
				shortest = j;
				check = true;
			} 	
		}
		
		if(check == false)
		{
			t++;
			continue;
		}
		rt[shortest]--;
		
		minm = rt[shortest];
		if(minm == 0)
		{
			minm = INT8_MAX;
		}
		
		if(rt[shortest] == 0)
		{
			complete++;
			
			finish_time = t + 1;
			
			process[shortest].waittime = finish_time - process[shortest].bursttime - process[shortest].arrivaltime;
			
			if(process[shortest].waittime < 0)
			{
				process[shortest].waittime = 0;
			}
		}
		t++;
	}
}

void avaragetime_sjf(struct process_details process[], int n, int *averagewaittime, int *averageturnaroundtime)
{
	int totalwaittime = 0;
	int totalturnaroundtime = 0;
	int i;
	
	destroy_values(process, n);
	waittime_sjf(process, n);
	turnaroundtime(process, n);

	cout << "SHORTEST JOB NEXT SCHEDULING :\n\n";
	cout << "Process id\t" << "Burst time\t" << "Arrival time\t";
	cout << "Waiting time\t" << "Turnaround time\t" << "Completion time\n";
	
	for(i = 0; i < n; i++)
	{
		totalwaittime = process[i].waittime + totalwaittime;
		totalturnaroundtime = process[i].turnaroundtime + totalturnaroundtime;
		process[i].completiontime = process[i].turnaroundtime + process[i].arrivaltime;
	}		
	
	for(i = 0; i < n; i++)
	{
		cout << process[i].processid << "\t\t" << process[i].bursttime << "\t\t";
		cout << process[i].arrivaltime << "\t\t" << process[i].waittime	<< "\t\t";
		cout << process[i].turnaroundtime << "\t\t" << process[i].completiontime << "\n";		
	}
	
	*averagewaittime = totalwaittime/n;
	*averageturnaroundtime = totalturnaroundtime/n;
	
	cout << "The average waiting time is : " << *averagewaittime << "\n";
	cout << "The average turnaround time is : " << *averageturnaroundtime << "\n\n";
}

void waittime_priority(struct process_details process[], int n)
{
	int servicetime[n];
	int i;
	
	process[0].waittime = 0;
	servicetime[0] = 0;
	for(i = 1; i < n; i++)
	{
		servicetime[i] = process[i-1].bursttime + servicetime[i-1];
		process[i].waittime = servicetime[i] - process[i].arrivaltime;
		
		if(process[i].waittime < 0)
		{
			process[i].waittime = 0;
		}
	}	
}


void avaragetime_priority(struct process_details process[], int n, int *averagewaittime, int *averageturnaroundtime)
{
	int totalwaittime = 0;
	int totalturnaroundtime = 0;
	int i;
	
	destroy_values(process, n);
	sort(process, n);
	waittime_priority(process, n);
	turnaroundtime(process, n);

	cout << "PRIORITY SCHEDULING :\n\n";
	cout << "Process id\t" << "Burst time\t" << "Arrival time\t";
	cout << "Waiting time\t" << "Turnaround time\t" << "Completion time\n";
	
	for(i = 0; i < n; i++)
	{
		totalwaittime = process[i].waittime + totalwaittime;
		totalturnaroundtime = process[i].turnaroundtime + totalturnaroundtime;
		process[i].completiontime = process[i].turnaroundtime + process[i].arrivaltime;
	}		
	
	for(i = 0; i < n; i++)
	{
		cout << process[i].processid << "\t\t" << process[i].bursttime << "\t\t";
		cout << process[i].arrivaltime << "\t\t" << process[i].waittime	<< "\t\t";
		cout << process[i].turnaroundtime << "\t\t" << process[i].completiontime << "\n";		
	}
	
	*averagewaittime = totalwaittime/n;
	*averageturnaroundtime = totalturnaroundtime/n;
	
	cout << "The average waiting time is : " << *averagewaittime << "\n";
	cout << "The average turnaround time is : " << *averageturnaroundtime << "\n\n";
}


void waittime_rr(struct process_details process[], int n)
{
	int t = 0;
	int rem_bt[n];
	
	for(int i = 0; i < n; i++)
	{
		rem_bt[i] = process[i].bursttime;
	}
	do
	{
		bool done = true;
		for(int i = 0; i < n; i++)
		{
			if(rem_bt[i] > 0)
			{
				done = false;
				if(rem_bt[i] > quantum)
				{
					t = t + quantum;
					rem_bt[i] -= quantum;
				}
			
				else 
				{
					t = t + rem_bt[i];
					process[i].waittime = t - process[i].bursttime;
					rem_bt[i] = 0;
				}
			}
		}
		
		
		
		if(done == true) 
			break;
	
	}while(1);
}


void avaragetime_rr(struct process_details process[], int n, int *averagewaittime, int *averageturnaroundtime)
{
	int totalwaittime = 0;
	int totalturnaroundtime = 0;
	int i;
	
	destroy_values(process, n);
	waittime_rr(process, n);
	turnaroundtime(process, n);

	cout << "ROUND ROBIN SCHEDULING :\n\n";
	cout << "Process id\t" << "Burst time\t";
	cout << "Waiting time\t" << "Turnaround time\t" << "Completion time\n";
	
	for(i = 0; i < n; i++)
	{
		totalwaittime = process[i].waittime + totalwaittime;
		totalturnaroundtime = process[i].turnaroundtime + totalturnaroundtime;
		process[i].completiontime = process[i].turnaroundtime + process[i].arrivaltime;
	}		
	
	for(i = 0; i < n; i++)
	{
		cout << process[i].processid << "\t\t" << process[i].bursttime << "\t\t";
		cout << process[i].waittime	<< "\t\t";
		cout << process[i].turnaroundtime << "\t\t" << process[i].completiontime << "\n";		
	}
	
	*averagewaittime = totalwaittime/n;
	*averageturnaroundtime = totalturnaroundtime/n;
	
	cout << "The average waiting time is : " << *averagewaittime << "\n";
	cout << "The average turnaround time is : " << *averageturnaroundtime << "\n\n";
}

void print_details(struct process_details process[], int n)
{
	int i;
	
	cout << "Process id\t" << "Burst time\t";
	cout << "Arrival time\t" << "Priority\n\n";

	for(i = 0; i < n; i++)
	{
		cout << process[i].processid << "\t\t" << process[i].bursttime << "\t\t";
		cout << process[i].arrivaltime << "\t\t" << process[i].priority << endl;
	}
	cout << "\n\n";
}

void compare(int averagewaittime[], int averageturnaroundtime[],char algorithms[][50], int n)
{
	int i;
	cout << "\nThe waittime time and turnaround time of different algorithms are : \n\n";
	cout << "Algorithm\t\t\t" << "Average Waitting Time\t\t" << "Average Turnaround Time\n" << endl;
	for(i = 0; i < n -1; i ++)
	{
		cout << algorithms[i] << "\t\t\t" << averagewaittime[i] << "\t\t\t";
		cout << averageturnaroundtime[i] << endl;
	}
}

void getalgorithms(char  algorithms[][50])
{
	char string[50];
	int i = 0;
	
	ifstream file("algorithms.txt");
	
    if(!file.is_open())
    {
	    cout << "Unable to open the file!!!";
	 	return;
	}
	
	while(file >> (algorithms[i]))
	{
		i++;
	}

}


int main()
{
	struct process_details process[maxsize];
	int size;
	int i = 0;
	int averagewaittime[maxsize] = {0};
	int averageturnaroundtime[maxsize] = {0};
	char algorithms[maxsize][50], ch;
	
	getalgorithms(algorithms);
	
	size = getdata(process);
	
	print_details(process, size);
	cout << "Press any key to continue : ";
	cin >> ch;
	cout << "\n\n";
	avaragetime_fcfs(process, size, (averagewaittime + i), (averageturnaroundtime + i));
	i++;
	cout << "Press any key to continue : ";
	cin >> ch;
	cout << "\n\n";
	avaragetime_sjf(process, size, (averagewaittime + i), (averageturnaroundtime + i));
	i++;
	cout << "Press any key to continue : ";
	cin >> ch;
	cout << "\n\n";
	avaragetime_rr(process, size, (averagewaittime + i), (averageturnaroundtime + i));
	i++;
	cout << "Press any key to continue : ";
	cin >> ch;
	cout << "\n\n";
	avaragetime_priority(process, size, (averagewaittime + i), (averageturnaroundtime + i));
	cout << "Press any key to continue : ";
	cin >> ch;
	cout << "\n\n";
	compare(averagewaittime, averageturnaroundtime, algorithms, size);
	return 0;
}


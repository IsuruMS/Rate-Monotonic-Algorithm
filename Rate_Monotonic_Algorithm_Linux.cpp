#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
using namespace std;

int tasks[2][100];
int timeline[2][100];
int taskCounts[100];
int noOfProcesses;
int LCM;
bool success;

int findLCM(int t[], int tSize)
{
	int initialArray[100];
	for (int i = 0; i < tSize; i++)
	{
		initialArray[i] = t[i];
	}
	int index, m, x, b = 1;
	while (b == 1)
	{
		b = 0;
		x = t[0];
		m = t[0];
		index = 0;
		for (int i = 0; i < tSize; i++)
		{
			if (x != t[i])
			{
				b = 1;
			}
			if (m > t[i])
			{
				m = t[i];
				index = i;
			}
		}
		if (b == 1)
		{
			t[index] = t[index] + initialArray[index];
		}
	}
	return t[0];
}

int priorityProcess()
{
	// this function returns the id of the process that is in the ready state
	// that is the process with the highest priority which is with the lowest period
	//	it returns -1 if there are no processes to execute
	int priority = -1, period = LCM;
	for (int i = 0; i < noOfProcesses; i++)
	{
		if (timeline[0][i] != 0)
		{
			if (period > timeline[1][i])
			{
				priority = i;
				period = timeline[1][i];
			}
		}
	}
	return priority;
}


void schedule()
{
	// the algorithm is modified as below
	// step 1 - find the task with the highest priority and check for schedulability
	// step 2 - push it into a vector which is used to keep the tasks that were successfully scheduled
	// step 3 - compare the doneTasks vector with the processes and burst times
	// logic of step 3
	//    * if a task with Period 2 and burst time 1 and period 4 and burst time 2 are to be shceduled
	//    * it must be within 4 seconds
	//    * inside the 4 seconds the t0 must have to be executed twice for 1 time unit each. that is 2 in total.
	//    * and the other 1 time that means 2 another time units
	//    * that means inside the doneTasks vector there must be 2 entries 0f task0 and 2 entries of task1
	// if this logic returns true and because the rate monotonic algorithm is implemented then we can say the tasks are schedulable
	
	int t, priority;
	vector<int> doneTasks;
	//intializing timeline
	for (int i = 0; i < noOfProcesses; i++)
	{
		timeline[0][i] = tasks[0][i];
		timeline[1][i] = tasks[1][i];
	}

	for (t = 0; t < LCM; t++)
	{
		for (int i = 0; i < noOfProcesses; i++)
		{
			if (timeline[0][i] > timeline[1][i])
			{
				system("cls");
				printf("\nthe system is not schedulable because the task %d cant finish its job in its specified period", i+1);
				return;
			}
		}
		// get the task with the highest priority
		priority = priorityProcess();

		if (priority != -1)		// there is a task ready to be executed
		{
			printf("\n(%d-%d)| t%d", t, t + 1, priority+1);
			doneTasks.push_back(priority);
			sort(doneTasks.begin(), doneTasks.end());
			for (int a = 0; a < noOfProcesses; a++)
			{
				taskCounts[a] = (count(doneTasks.begin(), doneTasks.end(), a));
			}
		}
		else		// there is no task in ready state
		{
			printf("\n(%d-%d)", t, t + 1);
		}

		timeline[0][priority]--;		// after one second of the burst time is executed remaining burst time is (previous burst time)--
		
		// update period time
		for (int i = 0; i < noOfProcesses; i++)
		{
			timeline[1][i]--;
			if (timeline[1][i] == 0)
			{
				timeline[0][i] = tasks[0][i];
				timeline[1][i] = tasks[1][i];
			}
		}
	}
}
int main()
{
	int x, help[100];
	cout << "Number of processes	:	";
	cin >> noOfProcesses;

	cout << "Please enter the data in the following order(space seperated)\n\n<Burst Time> <Period>\nand press enter" << endl << endl;
	for (int i = 0; i < noOfProcesses; i++)
	{
		cout << "task " << i + 1 << " : ";
		cin >> tasks[0][i] >> tasks[1][i];
	}

	float sum = 0;
	for (int i = 0; i < noOfProcesses; i++)
	{
		sum = sum + ((float)tasks[0][i] / (float)tasks[1][i]);
	}
	float reciprocal = 1.0 / noOfProcesses;
	float u = noOfProcesses * (pow(2, reciprocal) - 1);
	if (sum < u)
	{
		printf("\nThe system is surely schedulable because the sum of Burst time/Period = %f < %f\n", sum, u);
	}
	else
	{
		printf("sum of Burst time/Period = %f > %f\nBut This is not a nescessary Factor. Therefore the chart has to be drawn", sum, u);
	}
	// getting LCM...
	for (int i = 0; i < noOfProcesses; i++)
	{
		help[i] = tasks[1][i];
	}
	LCM = findLCM(help, noOfProcesses);
	// start scheduling
	schedule();
	for (int a = 0; a < noOfProcesses; a++)
	{
		if (taskCounts[a] == ((LCM * tasks[0][a]) / tasks[1][a]))
			success = true;
		else
			success = false;
	}
	if (success)
		printf("\n\nAll the Tasks are Successfully Scheduled\n\n");
	else
		printf("\nTasks are not successfully scheduled\n\n");

	system("pause");
}
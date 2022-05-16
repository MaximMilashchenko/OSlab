#include <iostream>
#include <vector>
#include <string>

int main()
{
    int nProcessesQueue = 0, quantum = 0, sum = 0, tmp = 0, t1 = 0, t2 = 0;
    float wait = 0, exec = 0;

    std::cout << "Enter a time quantum: ";
    std::cin >> quantum;
    std::cout << "Enter the number of processes in the queue: ";
	std::cin >> nProcessesQueue;

    std::vector<int> proc(nProcessesQueue);
    std::vector<int> count(nProcessesQueue);
    std::vector<int> posl(nProcessesQueue);
    char** gant = (char**)calloc(nProcessesQueue, sizeof(char*));

    for (int i = 0; i < nProcessesQueue; i++)
	{
		std::cout << "Enter the processing time of the " << i + 1 << " process: ";
		std::cin >> proc[i];
        sum = sum + proc[i];
		count[i] = 0;
		posl[i] = i;
		gant[i] = (char*)calloc(sum, sizeof(char));;
	}

	for (int i = 0; i < sum; i++)
	{
		if (t1 == 0)
		{
			t2 = proc[0];
			tmp = posl[0];
			for (int j = 0; j < nProcessesQueue - 1; j++)
			{
				proc[j] = proc[j + 1];
				posl[j] = posl[j + 1];
			}
			proc[nProcessesQueue - 1] = t2;
			posl[nProcessesQueue - 1] = tmp;
			t1 = quantum;
		}
		if (proc[0] > 0)
		{
			proc[0]--;
			gant[posl[0]][count[posl[0]]] = '+';
			count[posl[0]]++;
			exec++;
			for (int j = 1; j < nProcessesQueue; j++)
			{
				gant[posl[j]][count[posl[j]]] = '-';
				count[posl[j]]++;
				wait++;
			}
		}
		else
		{
			while (proc[0] <= 0)
			{
				t2 = proc[0];
				tmp = posl[0];
				for (int j = 0; j < nProcessesQueue - 1; j++)
				{
					proc[j] = proc[j + 1];
					posl[j] = posl[j + 1];
				}
				proc[nProcessesQueue - 1] = t2;
				posl[nProcessesQueue - 1] = tmp;
			}
			proc[0]--;
			gant[posl[0]][count[posl[0]]] = '+';
			count[posl[0]]++;
			exec++;
			for (int j = 1; j < nProcessesQueue; j++)
			{
				gant[posl[j]][count[posl[j]]] = '-';
				count[posl[j]]++;
				wait++;
			}
			t1 = quantum;
		}
		t1--;
	}
	for (int i = 0; i < nProcessesQueue; i++) gant[i][count[i]] = '\0';
	exec = exec / nProcessesQueue; wait = wait / nProcessesQueue;
    std::cout   << "FCFS algorithm: " << std::endl
                << "Average waiting time: " << wait << std::endl
                << "Average execution time: " << exec << std::endl
                << "Total execution time: " << sum << std::endl
                << "Gantt Chart: " << std::endl;
	for (int i = 0; i < nProcessesQueue; i++)
    {
		std::cout << i + 1 << " process " <<  gant[i] << std::endl;
    }
    
    return 0;
}
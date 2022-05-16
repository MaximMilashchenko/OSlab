#include <iostream>
#include <vector>

namespace
{
struct Process
{
	int ID;
	int TimeProcess;
	int QuantProcessors;
	int Status;
};

void SJF(std::vector<Process>& Processes, int CountProcess)
{
	int HelpTime = 0, HelpID = 0, HelpQuantProcessors = 0;;
	bool noSwap;

	for (int i = CountProcess - 1; i >= 0; i--)
	{
		noSwap = 1;
		for (int j = 0; j < i; j++)
		{
			if (Processes[j].TimeProcess > Processes[j + 1].TimeProcess)
			{
				HelpID = Processes[j].ID;
				HelpTime = Processes[j].TimeProcess;
				HelpQuantProcessors = Processes[j].QuantProcessors;

				Processes[j].ID = Processes[j + 1].ID;
				Processes[j].TimeProcess = Processes[j + 1].TimeProcess;
				Processes[j].QuantProcessors = Processes[j + 1].QuantProcessors;

				Processes[j + 1].ID = HelpID;
				Processes[j + 1].TimeProcess = HelpTime;
				Processes[j + 1].QuantProcessors = HelpQuantProcessors;
				noSwap = 0;
			}
		}
		if (noSwap == 1)
			break;
	}
}

int Available(std::vector <int>& Processors, int CountProcessors)
{
	int count = 0;
	for (int i = 0; i < CountProcessors; i++)
	{
		if (Processors[i] == 0)
			count++;
	}
	return count;
}

int FindFree(std::vector <int>& Processors, int CountProcessors, int CurrentID, int HelpNumber, int CurrentStatus)
{
	if (CurrentStatus == 1)
	{
		for (HelpNumber; HelpNumber < CountProcessors; HelpNumber++)
			if (CurrentID == Processors[HelpNumber] - 1)
				return HelpNumber;
	}
	else
	{
		for (HelpNumber; HelpNumber < CountProcessors; HelpNumber++)
			if (Processors[HelpNumber] == 0)
				return HelpNumber;
	}
}

void PrintGant(std::vector <std::vector<int>>& Gant, int CountProcessors, int Iteration)
{
	for (int i = 0; i < CountProcessors; i++)
	{
		std::cout << i + 1 << " CPU: ";
		for (int j = 0; j < Iteration - 1; j++)
		{
			if (Gant[i][j] > 0)
				std::cout << Gant[i][j] << " ";
			else
				std::cout << "_ ";
		}
		std::cout << std::endl;
	}
}
}

using namespace std;

int main(void)
{
	int CountProcessors = 1, CountProcess = 0, Algorithm = 0, Iteration = 0, Completed = 0, t = 0, CurrentProcessors = 0;

	cout << "Enter number of processes: ";
	cin >> CountProcess;

    std::vector <std::vector<int>> Gant(1, std::vector<int>(Iteration + 1, 0));
    std::vector <int> Processors(CountProcessors, 0);
    std::vector <int> Average(CountProcess, 0);
    std::vector <int> HelpTime(CountProcess, 0);
	std::vector<Process> Processes(CountProcess, Process());

	for (int i = 0; i < CountProcess; i++)
	{
		Processes[i].ID = i;
		std::cout << "Enter time for " << i + 1 << " process" << std::endl;
		std::cin >> Processes[i].TimeProcess;
		if (CountProcessors != 1)
		{
			std::cout << "Enter the number of CPU required for " << i + 1 << " process" << std::endl;
			std::cin >> Processes[i].QuantProcessors;
			if (Processes[i].QuantProcessors > CountProcessors)
			{
				std::cout << "There are not so many processors available in the system, the maximum number is set by default - " << CountProcessors << std::endl;
				Processes[i].QuantProcessors = CountProcessors;
			}
		}
		else 
            Processes[i].QuantProcessors = 1;
		Processes[i].Status = 0;
		HelpTime[i] = Processes[i].TimeProcess;
	}

	std::cout << "Select the algorithm by pressing the desired key: 1 - FCFS algorithm, 2 - SJF algorithm" << std::endl;
	std::cin >> Algorithm;
	if (Algorithm != 1 && Algorithm != 2)
		std::cout << "You entered the wrong item, the FCFS algorithm will be executed by default";

	if (Algorithm == 2)
		SJF(Processes, CountProcess);

	while (Completed < CountProcess)
	{
		int CurrentID = 0;
		for (int i = 0; i < CountProcessors; i++)
		{
			if (Processors[i] < 0)
				Processors[i] = 0;
		}
		while (t < CountProcess)
		{
			CurrentID = Processes[t].ID;
			CurrentProcessors = Available(Processors, CountProcessors);
			if ((Processes[CurrentID].Status == 0 && Processes[CurrentID].QuantProcessors <= CurrentProcessors) || Processes[CurrentID].Status == 1)
			{
				if (HelpTime[CurrentID] != 0)
				{
					int c = 0, NumberProcessor = 0, HelpNumber = 0;
					while (c < Processes[CurrentID].QuantProcessors)
					{
						NumberProcessor = FindFree(Processors, CountProcessors, CurrentID, HelpNumber, Processes[CurrentID].Status);
						Gant[NumberProcessor][Iteration] = CurrentID + 1;
						Processors[NumberProcessor] = CurrentID + 1;
						c++;
						HelpNumber = NumberProcessor + 1;
					}

					if (Processes[CurrentID].Status != 1)
						CurrentProcessors = CurrentProcessors - Processes[CurrentID].QuantProcessors;
					HelpTime[CurrentID]--;
					if (HelpTime[CurrentID] == 0)
					{
						for (int i = 0; i < CountProcessors; i++)
							if (Processors[i] - 1 == CurrentID)
								Processors[i] = -1;
					}
					Processes[CurrentID].Status = 1;
					t++;
				}
				else
				{
					Processes[CurrentID].Status = 2;
					Completed++;
					for (int i = 0; i < CountProcessors; i++)
						if (Processors[i] - 1 == CurrentID)
							Processors[i] = -1;
					t++;
				}
			}
			else if (Processes[CurrentID].Status == 2 || Processes[CurrentID].QuantProcessors > CurrentProcessors)
				t++;
		}
		t = 0;
		Iteration++;
		for (int i = 0; i < CountProcessors; i++)
            Gant[i].push_back(0);
	}

	int Waiting = -CountProcess, Implementation = 0, temp = 0;
	for (int i = 0; i < CountProcessors; i++)
	{
		for (int j = 0; j < Iteration - 1; j++)
		{
			if (Gant[i][j] >= 0)
				temp = Gant[i][j] - 1;
			if (Average[temp] == 0)
				Average[temp] = j + 1;
		}
	}

	for (int i = 0; i < CountProcess; i++)
	{
		Waiting = Average[i] + Waiting;
		Implementation = Average[i] + Processes[i].TimeProcess + Implementation - 1;
	}

	std::cout << "The result is in the form of a Gantt graph, where one column is one iteration; against the processor, the running processes are marked with numbers, and the <_> sign is waiting" << endl;
	PrintGant(Gant, CountProcessors, Iteration);
	std::cout   << "Additional calculations:" << std::endl 
                << "1. Time spent on the execution of all processes - "  << Iteration - 1 << std::endl
                << "2. Average total process execution time - " << Implementation / CountProcess << std::endl
                << "3. Average waiting time - " << Waiting / CountProcess << std::endl;

	return 0;
}
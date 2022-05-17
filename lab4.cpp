#include <iostream>
#include <vector>

struct Process
{
    int ID;
    int TimeProcess;
    int Priority;
    int TimeArrival;
    int Status;
};

int main(void)
{

    int CountProcess = 0, Iteration = 0, Completed = 0, Waiting = 0;

    std::cout << "Set the number of processes: ";
    std::cin >> CountProcess;
    std::vector<Process> Processes(CountProcess, Process());
    std::vector<std::vector<int>> Gant(CountProcess, std::vector<int>(1));

    for (int i = 0; i < CountProcess; i++) 
    {
        std::cout << "Set the time for execution " << i + 1 << " process: ";
        std::cin >> Processes[i].TimeProcess;
        std::cout << "Set the time of receipt " << i + 1 << " process: ";
        std::cin >> Processes[i].TimeArrival;
        std::cout << "Set the priority for " << i + 1 << " process: ";
        std::cin >> Processes[i].Priority;
        Processes[i].ID = i;
    }

    int CurrentID = 0;
    while (Completed < CountProcess)
    {
        int CurrentPriority = 0;
        Iteration++;
        for (int i = 0; i < CountProcess; i++)
        {
            if (Processes[i].Status != 2 && Processes[i].TimeArrival <= Iteration && Processes[i].Priority > CurrentPriority)
            {
                CurrentID = i;
                CurrentPriority = Processes[i].Priority;
            }
        }
        for (int i = 0; i < CountProcess; i++)
        {
            if (i != CurrentID && Processes[i].Status != 2 && Processes[i].TimeArrival <= Iteration)
            {
                Processes[i].Status = 1;
                Waiting++;
            }
            if (i != CurrentID)
            {
                Gant[i][Iteration - 1] = -1;
            }
        }
        if (Processes[CurrentID].Status == 2 || Processes[CurrentID].TimeArrival > Iteration)
        {
            Gant[CurrentID][Iteration - 1] = -2;
        }
        else
        {
            Processes[CurrentID].Status = 0;
            Processes[CurrentID].TimeProcess--;
            Gant[CurrentID][Iteration - 1] = 1;
            if (Processes[CurrentID].TimeProcess == 0)
            {
                Processes[CurrentID].Status = 2;
                Completed++;
            }
        }
        for (int i = 0; i < CountProcess; i++)
        {
            Gant[i].push_back(0);
        }
    }

    std::cout << "X - in progress, 0 - waiting, empty block - completed/not arrived yet:" << std::endl;
    for (int i = 0; i < CountProcess; i++)
    {
        std::cout << i + 1 << " process: ";
        for (int j = 0; j < Iteration; j++)
        {
            if (Gant[i][j] == 1)
                std::cout << "X ";
            else if (Gant[i][j] == -1)
                std::cout << "0 ";
        }
        std::cout << std::endl;
    }
    std::cout   << "Additional calculations:" << std::endl 
                << "1. Time spent executing all processes - " << Iteration << std::endl
                << "2. Average total process execution time - - " << (Waiting + Iteration) / CountProcess << std::endl
                << "3. Average waiting time - " << Waiting / CountProcess << std::endl;
    return 0;
}
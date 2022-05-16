#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <iomanip>

#include <time.h>

using namespace std;

namespace
{
template <class RandomAccessIterator>
void generatevector(RandomAccessIterator first, RandomAccessIterator last)
{
    std::random_device rnd_device;

    std::mt19937 mersenne_engine {rnd_device()};
    std::uniform_int_distribution<int> dist {1, 100};
    
    auto gen = [&dist, &mersenne_engine](){
                return dist(mersenne_engine);
            };
    generate(first, end, gen);;
}

struct Process
{
    int ID;
    int TimeProcess;

    Process(int id, int tp) : ID(id), TimeProcess(tp) {};
};

int Search(int Available, int Remaining, int Iteration, int n, int k, int Quantum, int CountProcess, std::vector<Process>& process, ofstream* out)
{
    int j = 0;
    int min = 1000;
    int t = 0;

    for (int i = 0; i < CountProcess; i++)
    {
        if (process[i].TimeProcess > min && process[i].TimeProcess > 0)
        {
            min = process[i].TimeProcess;
        }
    }

    for (int i = 0; i < CountProcess; i++)
    {
        if ((process[i].TimeProcess) > 0 && (Available - (process[i].TimeProcess)) < min && (Available - (process[i].TimeProcess)) >= 0)
        {
            min = Available - (process[i].TimeProcess);
            n = process[i].ID;
            k = process[i].TimeProcess;
            j = i;
        } 
        else
            t++;
        if (t == CountProcess)
        {
            Remaining = Available;
            if (n == 0) 
            {
            if (CountProcess < 21)
            {
                std::cout   << std::setw(10) << Iteration << " " << std::setw(18) << Quantum << " " << std::setw(15) << Available 
                            << " " << std::setw(25) << n << " " << std::setw(25) << k << " " << std::setw(25) << Remaining << std::endl;
            }
            else
            {
                *out << " " << Iteration << " " << Quantum << " " << Available << " " << n << " " << k << " " << Remaining << std::endl;
            }
            }
            return (Remaining);
        }
    }
    Remaining = Available - (process[j].TimeProcess);
    (process[j].TimeProcess) = -1;
    if (CountProcess < 21)
    {
        std::cout   << std::setw(10) << Iteration << " " << std::setw(18) << Quantum << " " << std::setw(15) << Available 
                    << " " << std::setw(25) << n << " " << std::setw(25) << k << " " << std::setw(25) << Remaining << std::endl;
    }
    else
    {
        *out << " " << Iteration << " " << Quantum << " " << Available << " " << n << " " << k << " " << Remaining << std::endl;
    }
    if (Remaining > 0)
    {
        Available = Remaining;
        Search(Available, Remaining, Iteration, n, k, Quantum, CountProcess, process, out);
    } 
    else
        return Remaining;
}

int menu()
{
    int action = 0;
    do
    {
        std::cout   << "Select type enter data" << std::endl
                    << "0 - manual data entry" << std::endl
                    << "1 - random generation" << std::endl
                    << "2 - reading from a file" << std::endl
                    << "3 - exit" << std::endl
                    << "Enter: ";
        std::cin >> action;
    } while (action < 0 || action > 3);

    return action;
}
};

int main(int argc, char** argv)
{
    if (argc != 3)
        return 0;

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string line;

    std::vector<Process> process;
    int Remaining = 0;
    int Iteration = 0;
    int Quantum = 0 ;
    int Available = 0;
    int CountProcess = 0;

    int n = 0;
    int k = 0;

    std::ofstream out(outputFile);
    std::ifstream in(inputFile);

    if (!in.is_open() && !out.is_open())
        return -1;

    switch (menu())
    {
        case 0:
        {
            std::cout << "Set the number of processes: ";
            std::cin >> CountProcess;
            for (int i = 0; i < CountProcess; i++)
            {
                int tp;
                std::cout << "Enter time for " << i << " process" << std::endl;
                std::cin >> tp;
                process.push_back(Process(i + 1, tp));
            }
            std::cout << "Enter Quantum: ";
            std::cin >> Quantum;
            break;
        }
        case 1:
        {
            CountProcess = rand() % 11;
            std::cout << "Generated processes: " << std::endl;
            for (int i = 0; i < CountProcess; i++)
            {
                process.push_back(Process(i + 1, rand() % 100 + 1));
                std::cout << process[i].ID << " " << process[i].TimeProcess << std::endl;
            }
            Quantum = rand() % 100 + 1;
            break;
        }
        case 2:
        {
            in >> CountProcess;
            std::cout << "Processes from file:" << std::endl;
            for (int i = 0; i < CountProcess; i++)
            {
                int tp;
                in >> tp;
                process.push_back(Process(i + 1, tp));
                out << process[i].ID << " " << process[i].TimeProcess << std::endl;
                out << "Iteration Number | Time Quantum | Total CPU time | Number of the selected process | Time of the selected process | Remaining CPU time" << std::endl;
            }
            std::cout << "Enter Quantum: ";
            std::cin >> Quantum;
            break;
        }
        case 3:
            return -1;
    }
    if (CountProcess < 21)
        std::cout << "Iteration Number | Time Quantum | Total CPU time | Number of the selected process | Time of the selected process | Remaining CPU time" << std::endl;
    
    for (;;)
    {
        int s = 0;
        for (int i = 0; i < CountProcess; i++)
        {
            if (process[i].TimeProcess <= 0)
                s++;
        }
        if (s == CountProcess)
        {
            std::cout << "The end!" << std::endl;
            return 0;
        }

        Available = Quantum + Remaining;
        Iteration++;
        Remaining = Search(Available, Remaining, Iteration, n, k, Quantum, CountProcess, process, &out);
    }
    return 0;
}


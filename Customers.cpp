#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include <mutex>
#include <atomic>

static const int NUM_THREADS = 10000;
std::atomic<int> door_counter;   //Number of people currently inside
std::atomic<int> thread_counter; //Number of threads currently created.
std::mutex mtx;

void process_customers();
void access_door(int thread_id);

class Ticket_lock
{
    int current, ticket_count = 0;

  public:
    int give_ticket();
    int now_serving();
    void serve(int thread_id);
};
Ticket_lock t_lock;

using namespace std;

int main()
{

    process_customers(); //Ticket Lock
    return 0;
}

void process_customers() //Ticket Lock
{
    thread threads[NUM_THREADS];
    thread_counter = 0;
    door_counter = 0;

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i] = thread(access_door, i);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads[i].join();
    }
}
void access_door(int thread_id)
{
    thread_counter++;                    // Atomic that holds the number of threads created. Prevents data races
    while (thread_counter < NUM_THREADS) //Create all threads first. Sleep yeilds this thread
    {
        sleep(1);
    }
    int my_ticket = t_lock.give_ticket();
    while (my_ticket < t_lock.now_serving())
    {
    }
    t_lock.serve(thread_id);
}

int Ticket_lock::give_ticket()
{
    mtx.lock();
    ticket_count++;
    cout << "Giving ticket " << ticket_count << endl;
    sleep(0.3);
    mtx.unlock();

    return ticket_count;
}
int Ticket_lock::now_serving()
{
    return current;
}
void Ticket_lock::serve(int thread_id)
{
    mtx.lock();
    cout << "Let in Customer number " << thread_id << endl;
    current++;
    door_counter++;
    mtx.unlock();
}
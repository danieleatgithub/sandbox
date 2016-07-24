/*
 * main.cpp
 *
 *  Created on: 27/mar/2016
 *      Author: daniele
 */

#include <Scheduler.hpp>

#include <cstdio>
#include <ctime>
#include <future>
#include <string>

#define MARKUSED(X)  ((void)(&(X)))

using namespace std;
using namespace shd;

void print_head() {
    std::time_t result = std::time(nullptr);
    string temp = std::asctime(std::localtime(&result));
    cout << "TRACE [" << std::this_thread::get_id() << "] "<< temp.substr(0,temp.size()-1);
}
void foo_sleep(int p) {
    print_head();
    cout << __PRETTY_FUNCTION__  << "<" << p << ">" << endl;
    if(p>0) this_thread::sleep_for(chrono::seconds(p));

}
void foo_promise(promise<int>& promise_r,int v) {
    print_head();
    cout << __PRETTY_FUNCTION__   << endl;
    int value = v;
    if(v > 0) this_thread::sleep_for(chrono::seconds(v));
    promise_r.set_value(move(value));
}
void foo_task1() {
    print_head();
    cout << __PRETTY_FUNCTION__  << endl;
}
void foo_task2() {
    print_head();
    cout << __PRETTY_FUNCTION__  << endl;
}
void foo_task3() {
    print_head();
    cout << __PRETTY_FUNCTION__  << endl;
}
void foo_task4() {
    print_head();
    cout << __PRETTY_FUNCTION__  << endl;
}void foo_chain_task2(Scheduler & sch) {
    auto now = std::chrono::system_clock::now();
    print_head();
    cout << __PRETTY_FUNCTION__   << endl;
    sch.ScheduleAt(now + std::chrono::seconds(1), [] { foo_task2(); });
}


int main() {


    Scheduler sch;
    promise<int> promise_1;
    future<int> future_1;
    int value_1 = 0;
    promise<int> promise_2;
    future<int> future_2;

    int value_2 = 0;
    MARKUSED(value_2); // Suppress warning unused-variable

    cb_t bind_fun2 = std::bind(foo_task2);
    cb_t bind_fun3 = std::bind(foo_sleep,-3);

    print_head();
    cout << "START" << endl;


    Task t1(std::chrono::seconds(2),foo_task1);
    Task t2(bind_fun2);
    Task t3(bind_fun3);
    Task t4([] () {
        foo_task4();
    });


    Task t5([&] { foo_promise(ref(promise_1),50); });
    Task t6([&] { foo_promise(ref(promise_2),60); });
    Task t7([&sch] { foo_sleep(7); });

    auto now = std::chrono::system_clock::now();
    sch.ScheduleEvery(t1);
    sch.ScheduleEvery(std::chrono::seconds(1),t2);
    sch.ScheduleEvery(std::chrono::seconds(4),t3);
    sch.ScheduleCancel(t2);
    sch.ScheduleAt(now + std::chrono::seconds(10), t4);
    sch.ScheduleAt(now + std::chrono::seconds(6), t5);
    sch.ScheduleAt(now + std::chrono::seconds(12), [&] { sch.ScheduleCancel(t3); });
    sch.ScheduleAt(now + std::chrono::seconds(13), [&] {
    	cout << "TRACE RESCHEDULE" << endl;
    	sch.ScheduleEvery(std::chrono::seconds(4),t1);
    });
    sch.ScheduleAt(now + std::chrono::seconds(5), [&sch] { foo_chain_task2(sch); });
    sch.ScheduleAfter(std::chrono::seconds(5), [] () { std::cout << "TRACE hello" << endl; });


    print_head();
    cout << "get future" << endl;
    future_1 = promise_1.get_future();
    print_head();
    cout << "get value" << endl;
    value_1 = future_1.get();
    print_head();
    cout << "value_1=" << value_1 << " wait=" << sch.getWaitingSize() << " running=" << sch.getRunningSize() <<endl;

    getchar();

    return 0;
}



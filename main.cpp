#include <iostream>
#include <queue>
#include "TimedAction_Types/I_TimedAction.hpp"
#include "TimedAction_Types/TimedAction.hpp"
#include "Cron/Cron.hpp"
#include <thread>
#include "Scheduler.hpp"


void sayHallo(std::uint32_t &count){

    std::cout << "Hallo " << count << std::endl;
    ++count;
    std::cout << "Hallo nr." << std::to_string(count) << std::endl;
}

void onAction(const std::string_view &value){
    std::cout << "onAction " << value << std::endl;
}

void onInterval(const std::string_view &value){
    std::cout << "onInterval " << value << std::endl;
}

void onEnd(const std::string_view &value){
    std::cout << "onEnd " << value << std::endl;
}

void test(std::string_view const& value){
    std::cout << value << std::endl;
}


int main() {

//    auto cron = Cron();
//    cron.seconds(1)
//        .minutes(1)
//        .hours(1)
//        .daysOfMonth(1)
//        .months(1)
//        .daysOfWeek("mon-fri")
//        .years("2021");

    std::uint32_t count = 0;
    auto job = TimedAction<std::uint32_t, std::string, std::string, std::string>(
            "Hallo",
            sayHallo,
            count,
            std::chrono::seconds(1)
    );

    std::string actionValue = "actionValue";
    std::string intervalValue = "intervalValue";
    std::string endValue = "endValue";

    job.setOnAction(onAction, (std::string &) actionValue);
//    job.setOnInterval(onInterval, (std::string &) intervalValue);
    job.setOnEnd(onEnd, (std::string &) endValue);


    std::string value = "Hallo_from_test";

    auto newJob = SmartTimedAction(
            "Hallo",
            test,
            value,
            std::chrono::seconds(1)
    );

    auto scheduler = Scheduler::get_instance();

    scheduler.add(&job);
    scheduler.add(&newJob);

    scheduler.start();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::vector<I_TimedAction> jobs;



    return 0;
}

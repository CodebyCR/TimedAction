#include <iostream>
//#include <queue>
//#include "TimedAction_Types/I_TimedAction.hpp"
//#include "TimedAction_Types/TimedAction.hpp"
#include "Cron/Cron.hpp"
#include <thread>
//#include "Scheduler.hpp"
#include "StringUtils.hpp"
#include "CronPart.hpp"
#include "Cron/CronInterpreter.hpp"


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

    const uint numThreads = std::thread::hardware_concurrency();

    std::cout << "Number of threads: " << numThreads << std::endl;

    // working
    // 5
    // */5
    // *
    // 0-5          -> last value is excluded
    // 0,1,2,3,4,5
    // 1-23/2

    // works fine
    // auto seconds = CronPart("second", "1-23/2");
    // auto minutes = CronPart("minute", "0-5");
    // auto hours = CronPart("hour", "1-13/2");
    // auto dayOfMonth = CronPart("day", "1-31/2");



//    // TODO: right days in month && leap years
//    auto months = CronPart("year", "*");
////    auto weekdays = CronPart("weekday", "*/5");
//
//    for (const auto &timeSeconds : months.getTimes()){
//        // To String
//        std::cout << "Count of seconds: " << timeSeconds.count() << std::endl;
//    }

    auto everyDay = Cron("0 0 0 1 * 3 2021-2023");
    CronInterpreter::print_time_points(everyDay);






//
//    CronInterpreter interpreter("0 */5 12 15,16 0-6");
//    std::cout << "test" << std::endl;
//    auto next_trigger_time = interpreter.NextTriggerTime();


    // next_trigger_time to string
//    std::time_t t = std::chrono::system_clock::to_time_t(next_trigger_time);
//    std::cout << std::ctime(&t) << std::endl;


//    CronPart part = CronPart("Minute", "0-59");




//    std::string cronTest = "1-12 || * || */2 ";
//
//    auto components = StringUtils::split_by_any_of( cronTest, "-*/");
//
//    for (auto &component : components) {
//        std::cout << component << std::endl;
//    }

//    auto cron = Cron();
//    cron.seconds(1)
//        .minutes(1)
//        .hours(1)
//        .daysOfMonth(1)
//        .months(1)
//        .daysOfWeek("mon-fri")
//        .years("2021");

//    std::uint32_t count = 0;
//    auto job = TimedAction<std::uint32_t, std::string, std::string, std::string>(
//             "Hallo",
//             sayHallo,
//                count,
//            std::chrono::seconds(1)
//    );
//
//    std::string actionValue = "actionValue";
//    std::string intervalValue = "intervalValue";
//    std::string endValue = "endValue";
//
//    job.setOnAction(onAction, (std::string &) actionValue);
////    job.setOnInterval(onInterval, (std::string &) intervalValue);
//    job.setOnEnd(onEnd, (std::string &) endValue);
//
//
//    std::string value = "Hallo_from_test";
//
//    auto newJob = SmartTimedAction(
//            "Hallo",
//            test,
//            value,
//            std::chrono::seconds(1)
//    );

//    auto everyDay = Cron()
//            .hours(12);
//
//    std::cout << "Every day at 12:00 " << everyDay.toString() << std::endl;




//    auto scheduler = Scheduler::get_instance();
//
//    scheduler.add(&job);
//    scheduler.add(&newJob);
//
////    scheduler.start();
//
//    std::this_thread::sleep_for(std::chrono::seconds(10));
//
//    std::vector<I_TimedAction> jobs;





    return 0;
}

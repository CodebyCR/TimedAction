#include <iostream>
//#include <queue>
#include "TimedAction_Types/I_TimedAction.hpp"
//#include "TimedAction_Types/TimedAction.hpp"
#include "TimedAction_Types/TimedAction.hpp"
#include "Cron/Cron.hpp"
#include <thread>
#include <memory>
//#include "Scheduler.hpp"

#include "Cron/CronInterpreter.hpp"
#include "Scheduler/Scheduler.hpp"
#include "Container/AsyncQueue.hpp"
#include "TimedAction_Types/Light_TimedAction.hpp"


void sayHallo(std::uint32_t &count) {
    ++count;
    std::cout << "Hallo nr." << std::to_string(count) << std::endl;
}

void onAction(const std::string_view &value) {
    std::cout << "onAction " << value << std::endl;
}

void onInterval(const std::string_view &value) {
    std::cout << "onInterval " << value << std::endl;
}

void onEnd(const std::string_view &value) {
    std::cout << "onEnd " << value << std::endl;
}

void test(std::string_view const &value) {
    std::cout << value << std::endl;
}


void test_async_queue(){
    auto queue = AsyncQueue<int>();

    // Registrieren einer Callback-Funktion, die aufgerufen wird, wenn ein Element in die Warteschlange eingefügt wird.
    queue.on_subscribe([](int item)
                    { std::cout << "Element " << item << " wurde in die Warteschlange eingefügt." << std::endl; });

    // Fügen Sie Elemente in die Warteschlange ein.
    queue.push(1);
    queue.push(2);
    queue.push(3);

    queue.on_listen([](int item)
                 { std::cout << "Element " << item << " wurde aus der Warteschlange entfernt." << std::endl; });

    std::cout << "Elemente in der Warteschlange: " << queue.size() << std::endl;
    // Entfernen Sie Elemente aus der Warteschlange.
    queue.clear();
    std::cout << "Elemente in der Warteschlange: " << queue.size() << std::endl;

    for(auto i = 0; i < 100; ++i){
        queue.push(i);
    }

    std::cout << "Elemente in der Warteschlange: " << queue.size() << std::endl;

    for (auto queueItem : queue) {
        std::cout << queueItem << std::endl;
        queue.pop();
    }

}

 auto main() -> int {

    // TODO: make runnable
     auto& scheduler = Scheduler::get_instance();

     std::uint32_t count = 2;
     std::string str = "test";

     std::cout << "-- test 1" << std::endl;

     auto c_cron = Cron({.second="0",
                         .minute="*/2",
                         .hour="*",
                         .dayOfMonth="28",
                         .month="4",
                         .weekday="*",
                         .year="*"});


     auto job = SmartTimedAction("My_first_Job",
                                 test,
                                 str,
                                 c_cron
     );


     auto execution_times = job.get_execution_times();

      std::cout << CronInterpreter::get_info(execution_times);


     std::cout << "-- test 2" << std::endl;


     scheduler.start();
     std::cout << "-- test 3" << std::endl;
     std::this_thread::sleep_for(std::chrono::seconds(2));


     scheduler.add(&job);


     // waiting for enter 'q'
    char c;
    std::cin >> c;

     while(c != 'q'){
         std::cin >> c;
     }





     std::cout << "-- test 4" << std::endl;
     scheduler.stop();


    // auto action_ptr = std::make_shared<I_TimedAction>(job);
     //scheduler.add(&job);


     return 0;
 }

//int main() {
//
//    const uint numThreads = std::thread::hardware_concurrency();
//
//    test_async_queue();
//
//    std::cin.get();
//
//    std::cout << "Number of threads: " << numThreads << std::endl;
//
//    // working
//    // 5
//    // */5
//    // *
//    // 0-5          -> last value is excluded
//    // 0,1,2,3,4,5
//    // 1-23/2
//
//
////    auto someCron = Cron("0 30 */2 1 3 * 2021-2023");
//
////    auto someCron = std::to_cron("0 30 */2 1 3 * 2021-2023");
//
//    auto someCron = Cron({
//        .second = "0",
//        .minute = "30",
//        .hour = "*/2",
//        .dayOfMonth = "15",
//        .month = "*/2",
//        .weekday = "mon,fri,sat",
//        .year = "2021-2024"});
//
//        auto executions = someCron.get_execution_times();
//        CronInterpreter::pretty_print(executions);
//
////    const auto every_minute = CronExpression::everyMinute();
////    const auto every_five_minutes = CronExpression::everyFiveMinutes();
////    const auto every_ten_minutes = CronExpression::everyTenMinutes();
////    const auto every_fifteen_minutes = CronExpression::everyFifteenMinutes();
////    const auto every_thirty_minutes = CronExpression::everyThirtyMinutes();
////    const auto every_hour = CronExpression::everyHour();
////    const auto every_second_hour = CronExpression::everyTwoHours();
////    const auto every_day = CronExpression::everyDay();
////    const auto every_second_day = CronExpression::everyTwoDays();
////    const auto every_week = CronExpression::everyWeek();
////    const auto every_month = CronExpression::everyMonth();
//
//
//
////    auto daily = "0 0 0 * * * 2024"_cron;
//
////    std::cout << "will executed earlier: " << *every_day << std::endl;
//
////    auto timePoints = every_five_minutes.get_execution_times();
////    std::cout << CronInterpreter::get_info(timePoints);
//
////    std::cout << "SomeCron: " << someCron << std::endl;
//
////    auto interpreter = CronInterpreter(someCron);
////
////    auto nextTriggerTime = interpreter.NextTriggerTime();
////
////    std::cout << "Next trigger time: " << nextTriggerTime << std::endl;
////
////    return 0;
////}
//
//
////    CronInterpreter interpreter("0 */5 12 15,16 0-6");
////    std::cout << "test" << std::endl;
////    auto next_trigger_time = interpreter.NextTriggerTime();
//
////    next_trigger_time to string
////    std::time_t t = std::chrono::system_clock::to_time_t(next_trigger_time);
////    std::cout << std::ctime(&t) << std::endl;
//
//
//
////    std::string cronTest = "1-12 || * || */2 ";
////
////    auto components = StringUtils::split_by_any_of( cronTest, "-*/");
////
////    for (auto &component : components) {
////        std::cout << component << std::endl;
////    }
//
////    auto cron = Cron()
////            .seconds(1)
////        .minutes(1)
////        .hours(1)
////        .daysOfMonth(1)
////        .months(1)
////        .daysOfWeek("mon-fri")
////        .years("2021");
//
//    std::uint32_t count = 2;
//    auto ccron = Cron("0 */2 0 1 1 * 2024");
//    auto job = TimedAction<std::uint32_t, std::string, std::string, std::string>(
//             "Hallo",
//             sayHallo,
//             count,
//             ccron
//    );
//
//    job.start();
//
//    std::this_thread::sleep_for(std::chrono::minutes(10));
//
////    std::string actionValue = "actionValue";
////    std::string intervalValue = "intervalValue";
////    std::string endValue = "endValue";
////
////    job.setOnAction(onAction, (std::string &) actionValue);
////////    job.setOnInterval(onInterval, (std::string &) intervalValue);
////    job.setOnEnd(onEnd, (std::string &) endValue);
////
////
////    job.start();
//
////    std::string value = "Hallo_from_test";
////
////    auto newJob = SmartTimedAction(
////            "Hallo",
////            test,
////            value,
////            std::chrono::seconds(1)
////    );
//
////    auto everyDay = Cron()
////            .hours(12);
////
////    std::cout << "Every day at 12:00 " << everyDay.toString() << std::endl;
//
////    std::vector<std::tm> times = {
////            {.tm_sec = 0, .tm_min = 0, .tm_hour = 0, .tm_mday = 1, .tm_mon = 0, .tm_year = 2020},
////            {.tm_sec = 0, .tm_min = 0, .tm_hour = 0, .tm_mday = 15, .tm_mon = 11, .tm_year = 2019},
////            {.tm_sec = 0, .tm_min = 0, .tm_hour = 0, .tm_mday = 31, .tm_mon = 5, .tm_year = 2021},
////            {.tm_sec = 0, .tm_min = 0, .tm_hour = 0, .tm_mday = 22, .tm_mon = 7, .tm_year = 2022}
////    };
//
//
////    auto scheduler = Scheduler::get_instance();
////
////    scheduler.add(&job);
//////    scheduler.add(&newJob);
////
////    scheduler.start();
////
////    std::this_thread::sleep_for(std::chrono::seconds(10));
////    std::vector<I_TimedAction> jobs;
//
//    return 0;
//}

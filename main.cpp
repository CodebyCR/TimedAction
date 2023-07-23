#include <iostream>

#include "Cron/Cron.hpp"
#include "Interfaces/I_TimedAction.hpp"
#include "TimedAction_Types/Light_TimedAction.hpp"
#include "TimedAction_Types/TimedAction.hpp"
#include <thread>


#include "Container/AsyncQueue.hpp"
#include "Cron/CronInterpreter.hpp"
#include "Scheduler/Scheduler.hpp"
#include "Utilities/Logger.hpp"
#include <iomanip>
#include <ctime>
#include <codecvt>
#include <cstdlib>
#include "Utilities/Environment.hpp"

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


void test_async_queue() {
    auto queue = AsyncQueue<int>();

    // Registrieren einer Callback-Funktion, die aufgerufen wird, wenn ein Element in die Warteschlange eingefügt wird.
    queue.on_subscribe(
            [](int item) { std::cout << "Element " << item << " wurde in die Warteschlange eingefügt." << std::endl; });

    // Fügen Sie Elemente in die Warteschlange ein.
    queue.push(1);
    queue.push(2);
    queue.push(3);

    queue.on_listen(
            [](int item) { std::cout << "Element " << item << " wurde aus der Warteschlange entfernt." << std::endl; });

    std::cout << "Elemente in der Warteschlange: " << queue.size() << std::endl;
    // Entfernen Sie Elemente aus der Warteschlange.
    queue.clear();
    std::cout << "Elemente in der Warteschlange: " << queue.size() << std::endl;

    for (auto i = 0; i < 100; ++i) {
        queue.push(i);
    }

    std::cout << "Elemente in der Warteschlange: " << queue.size() << std::endl;

    for (auto queueItem: queue) {
        std::cout << queueItem << std::endl;
        queue.pop();
    }
}

void test_future_task(I_TimedAction &action) {
    std::future<Notification> task = action.finished();


    switch (const auto result = task.wait_for(std::chrono::milliseconds(10)); result) {
        case std::future_status::ready: {
            std::cout << "Watcher: finished " << action.getName() << std::endl;

            auto notification = task.get();
            // cast notification to JobLog

            //
            //            auto jobLog = dynamic_cast<JobLog&>(notification);
            //            jobLog.print();
        }
            break;

        case std::future_status::deferred: {
            std::cout << "Watcher: deferred for " << action.getName() << std::endl;
        }
            break;

        case std::future_status::timeout: {
            std::cout << "Watcher: timeout for " << action.getName() << std::endl;
        }
            break;

        default: {
            std::cout << "Watcher: unknown status for " << action.getName() << std::endl;
        }
    }
}


auto logger_test() {
    Logger logger(true);

    std::ostream logstream(&logger);
    logstream << "This line will be logged and printed to cout" << std::endl;

    // Schreibe einige weitere Zeilen in den Log-Stream und flushe den Logger
    logstream << "This line will be logged and printed to cout and log file" << std::endl;
    logger.flush();

    return 0;
}



auto json_parser_test() -> void {


}

auto main() -> int {

//    CronTest::test_all();
    json_parser_test();

//    constexpr std::u8string_view brand = u8"\x1B[31m"
//                               "===========================================================================\n"
//                               "Timed Action •\x1B[3m since 2022\x1B[0m\x1B[31m •\n"
//                               "Version: 0.4.0\n"
//                               "All rights reserved.\n"
//                               "Copyright © 2023. Christoph Rohde\n"
//                               "Licence: MIT\n"
//                               "===========================================================================\n\x1B[0m";

    // print brand
//    std::cout << brand << std::endl;

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    constexpr auto brand = U"\x1B[31m"
                           "===========================================================================\n"
                           "Timed Action •\x1B[3m since 2022\x1B[0m\x1B[31m •\n"
                           "Version: 0.4.0\n"
                           "All rights reserved.\n"
                           "Copyright © 2023. Christoph Rohde\n"
                           "Licence: MIT\n"
                           "===========================================================================\n\x1B[0m";
    std::cout << converter.to_bytes(brand) << std::endl;


    //logger_test();    // ! Failed


//    auto cron_try = std::to_cron("0 */1 * * 6 * 2023");


    // ! TODO:
    //  Month must be - 1




//    std::cout << "Cron-1-: "  << std::endl;

//    auto cache = cron_try.get_execution_times();
//
//    for (auto &item : cache) {
//        std::cout << item.tm_hour << std::endl;
//    }
//
//
////    CronInterpreter::pretty_print(cache);
//
//    CronInterpreter::get_info(cache);
//
//
//    std::cin.get();
////
////    std::cout << "Cron-2-: "  << std::endl;
//    CronInterpreter::get_info(cron_try.resume_execution_times()
//                                      .get_execution_times());
//
//    std::cin.get();
//
//    std::cout << "Cron-3-: " << std::endl;
//    CronInterpreter::get_info(cron_try.resume_execution_times()
//                                      .get_execution_times());
//
//    std::cin.get();

    auto c_cron = Cron({.second = "0",
                       .minute = "*/2",
                       .hour = "*",
                       .dayOfMonth = "21",
                       .month = "5",
                       .weekday = "*",
                       .year = "*"});

//    CronInterpreter::each_print(c_cron);


    std::cout << "c_cron" << std::endl;
    std::cout << c_cron << std::endl;
    std::cin.get();





    // TODO: make runnable
    auto &scheduler = Scheduler::get_instance();

    std::uint32_t count = 2;
    std::string str = "test";

    // every minute  cache only for the current day !

    //std::cout << CronInterpreter::get_info(cron_try.get_execution_times());


//    auto job2 = SmartTimedAction("My_second_Job",
//                                 test,
//                                 str,
//                                 cron_try);
//s
//    scheduler.add(&job2);




    //    auto c_cron = Cron("* */1 * * * * *");

    std::cout << c_cron << std::endl;

//    auto job = SmartTimedAction("My_first_Job",
//                                test,
//                                str,
//                                c_cron);
//
//    std::cout << job.getName() << std::endl;

    scheduler.start();
    std::this_thread::sleep_for(std::chrono::seconds(2));

//    scheduler.add(&job);


    // waiting for enter 'q'
    char c;
    std::cin >> c;

    while (c != 'q') {
        std::cin >> c;
    }


//    std::cout << scheduler.get_runtime_info() << std::endl;

    scheduler.stop();


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

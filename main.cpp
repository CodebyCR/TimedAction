

#include "TimedActionLib/Container/Action.hpp"
#include "TimedActionLib/Container/AsyncQueue.hpp"
#include "TimedActionLib/Container/Consumer.hpp"
#include "TimedActionLib/Cron/Cron.hpp"
#include "TimedActionLib/Exception/SchedulerException.hpp"
#include "TimedActionLib/Interfaces/ActionCapsule.hpp"
#include "TimedActionLib/Interfaces/I_TimedAction.hpp"
#include "TimedActionLib/Scheduler/Scheduler.hpp"
#include "TimedActionLib/TimedAction_Types/TimedAction.hpp"
#include "TimedActionLib/Utilities/Logger.hpp"

#include <codecvt>
#include <iomanip>
#include <iostream>
#include <thread>


void onAction(const std::string_view& value) {
    std::cout << "onAction " << value << std::endl;
    auto ym = std::chrono::year { 2022 } / std::chrono::July; // ? refactoring

   // auto duration = std::chrono::duration_cast<std::chrono::seconds>(ym - std::chrono::system_clock::now()); // for next execution
//    std::string msg = std::format("{:*^10}\n{:*>10}\nin{}!", "hello", "world", ym);
//    std::cout << msg;
}

void onInterval(const std::string_view& value) {
    std::cout << "onInterval " << value << std::endl;
}

void onEnd(const std::string_view& value) {
    std::cout << "onEnd " << value << std::endl;
}

void test(std::string_view const& value) {
    std::cout << value << std::endl;
}


void test_async_queue() {
    auto queue = AsyncQueue<std::uint32_t>();

    // Registrieren einer Callback-Funktion, die aufgerufen wird, wenn ein Element in die Warteschlange eingefügt wird.
    queue.on_subscribe([](std::uint32_t item) {
        std::cout << "Element " << item << " wurde in die Warteschlange eingefügt." << std::endl; });

    // Fügen Sie Elemente in die Warteschlange ein.
    queue.push(1);
    queue.push(2);
    queue.push(3);

    queue.on_listen(
            [](auto item) { std::cout << "Element " << item << " wurde aus der Warteschlange entfernt." << std::endl; });

    std::cout << "Elemente in der Warteschlange: " << queue.size() << std::endl;
    // Entfernen Sie Elemente aus der Warteschlange.
    queue.clear();
    std::cout << "Elemente in der Warteschlange: " << queue.size() << std::endl;

    for(auto i = 0; i < 100; ++i) {
        queue.push(i);
    }

    std::cout << "Elemente in der Warteschlange: " << queue.size() << std::endl;

    for(auto queueItem: queue) {
        std::cout << queueItem << std::endl;
        queue.pop();
    }
}

void test_future_task(I_TimedAction& action) {
    std::future<Message> task = action.finished();


    switch(const auto result = task.wait_for(std::chrono::milliseconds(10)); result) {
        case std::future_status::ready: {
            std::cout << "Watcher: finished " << action.getName() << std::endl;

            auto notification = task.get();

        } break;

        case std::future_status::deferred: {
            std::cout << "Watcher: deferred for " << action.getName() << std::endl;
        } break;

        case std::future_status::timeout: {
            std::cout << "Watcher: timeout for " << action.getName() << std::endl;
        } break;

        default: {
            std::cout << "Watcher: unknown status for " << action.getName() << std::endl;
        }
    }
}


//auto logger_test() {
//    Logger logger(true);
//
//    std::ostream logstream(&logger);
//    logstream << "This line will be logged and printed to cout" << std::endl;
//
//    // Schreibe einige weitere Zeilen in den Log-Stream und flushe den Logger
//    logstream << "This line will be logged and printed to cout and log file" << std::endl;
//    logger.flush();
//
//    return 0;
//}

auto test_consumer(Consumer<std::string_view> consumer) -> void {
    consumer.accept("Hello World");
}

auto test_consumer_call() -> void {
    auto consumer = Consumer<std::string_view>([&](std::string_view value) -> void {
        std::cout << value << std::endl;
    });

    test_consumer(consumer);
}


auto test_function() -> void {
    std::cout << "test_function" << std::endl;
}

auto main() -> int {
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

//    logger_test();

    auto cron_try = Cron("0 */1 * * 7 * 2023"); // bug: no execution if year is set?
    auto testCron = Cron("0 */2 * 27 7 * *");

    auto c_cron = Cron({
            .second = "0",
            .minute = "*/2",
            .hour = "*",
            .dayOfMonth = "28",
            .month = "7",
            .weekday = "*",
            .year = "*"});

//    std::cout << c_cron << std::endl;

    auto action = ActionCapsule{
            .name = "My first Job",
            .execution_timer = &c_cron,
            .action = []() {
                std::cout << "Hello World" << std::endl;
            }};

    auto scheduler = Scheduler::get_instance();
    scheduler->add(action);
    scheduler->start();

    // running...

    std::string command;

    while(true) {
        std::cin >> command;

        // add 'test_on_the_fly' '0 */2 * * 8 * *' 'Hey there!'

        if(command.contains("add")) {
            const auto values = StringUtils::save_split(command, ' ', '\'');
            std::for_each(values.begin(), values.end(), [](const auto& value) {
                std::cout << value << std::endl;
            });

            try {
                const auto actionValues = StringUtils::get_substring(command, '\'', '\'');

                auto cron = Cron(values[1]);
                auto on_the_fly_action = ActionCapsule{
                        .name = values[0],
                        .execution_timer = &cron,
                        .action = [&values]() -> void {
                            std::cout << values[2] << std::endl;
                        }};
                scheduler->add(on_the_fly_action);
            }catch (SchedulerException& e ) {
                std::cout << "Failed:\n" << e.what() << std::endl;
            }
        }

        if(command == "q") {
            break;
        }

        if(command == "info") {
            std::cout << scheduler->get_runtime_info() << std::endl;
        }
    }
    scheduler->stop();

    return 0;
}

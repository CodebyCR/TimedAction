# Working with Cron... _can be easy_

- You can also read the [Cron Format](Cron_Format.md), of this Library for more information.

## Initialization

### Use designated initializers for complex cron expressions
```C++
    auto someCron = Cron({.second = "0",
                          .minute = "30",
                          .hour = "*/2",
                          .dayOfMonth = "15",
                          .month = "*/2",
                          .weekday = "*",
                          .year = "2023-2024"});
                          
    // instead of            
    // auto someCron = Cron("0 30 */2 1 3 * 2021-2023");
```

### Or choose between this nice one-liners
```C++
    // Initialize a cron with a string
    auto cron = Cron("1 1 1 1 1 mon-fri 2021");

    /** Currently in process*/
    // String literals are also supported
    auto cron = "1 1 1 1 1 mon-fri 2021"_cron;
```

### Or few pre-built formats
```C++
    const auto every_minute = CronExpression::everyMinute();
    const auto every_five_minutes = CronExpression::everyFiveMinutes();
    const auto every_ten_minutes = CronExpression::everyTenMinutes();
    
    const auto every_fifteen_minutes = CronExpression::everyFifteenMinutes();
    const auto every_thirty_minutes = CronExpression::everyThirtyMinutes();
    const auto every_hour = CronExpression::everyHour();
    
    const auto every_second_hour = CronExpression::everyTwoHours();
    const auto every_day = CronExpression::everyDay();
    const auto every_second_day = CronExpression::everyTwoDays();
    
    const auto every_week = CronExpression::everyWeek();
    const auto every_month = CronExpression::everyMonth();
    
    // These are sheared pointers, for less memory usage.
```

## Operators overloading

> Note:
> <br/>The `>>` operator is overloaded to parse a cron string into a Cron object.
> <br/>The `<<` operator is overloaded to convert a Cron object into a cron string.

```C++
    // Basic cron format string
    auto rawCron = "1 1 1 1 1 mon-fri 2021";
    auto cron = Cron();
    
    // Parse a cron string into a Cron object
    rawCron >> cron; 
    
    // Convert a Cron object into a cron string
    std::cout << cron << std::endl;
    result: 1 1 1 1 1 mon-fri 2021
```

> ! Will be implemented in the future !
> <br/>
> Note: 
> <br/>The `==` operator is overloaded to compare the next execution times of two Cron objects.

```C++
    auto cron1 = Cron("1 1 1 1 1 mon-fri 2021");
    auto cron2 = Cron("1 1 1 1 1 mon-fri 2021");
    auto cron3 = Cron("1 1 1 1 1 mon-fri 2022");
    
    // Compare two Cron objects
    std::cout << (cron1 == cron2) << std::endl;
    // result: 1
    
    std::cout << (cron1 == cron3) << std::endl;
    // result: 0
```

> Note: 
> <br/>The `>`  and `<` operators are overloaded to compare the __next__ execution time.
> <br/>(smaller means next execution time is sooner)

```C++
    auto cron1 = Cron("1 1 1 1 1 mon-fri 2021");
    auto cron2 = Cron("1 1 1 1 1 mon-fri 2021");
    auto cron3 = Cron("1 1 1 1 1 mon-fri 2022");
    
    // Compare the next execution time of two Cron objects
    std::cout << (cron1 > cron2) << std::endl;
    // result: 0
    
    std::cout << (cron1 < cron3) << std::endl;
    // result: 1
```

```C++
    auto cron1 = Cron("1 1 1 1 1 mon-fri 2021");
    auto cron2 = Cron("1 1 1 1 1 mon-fri 2021");
    auto cron3 = Cron("1 1 1 1 1 mon-fri 2022");
    
    // Compare the next execution time
    std::cout << (cron1 > cron2) << std::endl;
    // result: 0
    
    std::cout << (cron1 < cron3) << std::endl;
    // result: 1
```

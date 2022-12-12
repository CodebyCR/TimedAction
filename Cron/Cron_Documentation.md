# Working with Cron... _can be easy_

## Initialization

### Use a builder pattern
```C++
    auto cron = Cron()
                .seconds(1)
                .minutes(1)
                .hours(1)
                .daysOfMonth(1)
                .months(1)
                .daysOfWeek("mon-fri")
                .years("2021");
```

### Or choose between this nice one-liners
```C++
    // Initialize a cron with a string
    auto cron = Cron("1 1 1 1 1 mon-fri 2021");

    // Parse a cron string
    auto cron = Cron.parse("1 1 1 1 1 mon-fri 2021");

    // String literals are also supported
    auto cron = "1 1 1 1 1 mon-fri 2021"_cron;
```

### Or few pre-built formats
```C++
    auto cron = Cron::everyMinute();
    auto cron = Cron::everyHour();
    auto cron = Cron::everyDay();
    auto cron = Cron::everyWeek();
    auto cron = Cron::everyMonth();
    auto cron = Cron::everyYear();
```

## Operators overloading
```C++
    auto cron = Cron::everyMinute();

    // Use Chrono literals
    cron *= 2_hours; // Multiply by 2
```
> Note:<br/>The `>>` operator is overloaded to parse a cron string into a Cron object.<br/>The `<<` operator is overloaded to convert a Cron object into a cron string.

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
> Note: The `==` operator is overloaded to compare two Cron objects.

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

> Note: The `>`  and `<` operators are overloaded to compare the __next__ execution time.<br/> (smaller means next execution time is sooner)

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
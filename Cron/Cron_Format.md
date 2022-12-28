# Cron Format

## The Cron Format is a string that contains 6 to 7 part.

```Text
 ┌───────────────────────── 1) seconds [optional]
 │ ┌─────────────────────── 2) minutes
 │ │ ┌───────────────────── 3) hours
 │ │ │ ┌─────────────────── 4) day of the month
 │ │ │ │ ┌───────────────── 5) month
 │ │ │ │ │ ┌─────────────── 6) day of the week
 │ │ │ │ │ │ ┌───────────── 7) year
 │ │ │ │ │ │ │
 │ │ │ │ │ │ │
 * * * * * * *
```

## Possilbe values
- [Special characters](#Special-characters)

## The Parts
- [Seconds](#Seconds)
- [Minutes](#Minutes)
- [Hours](#Hours)
- [Day of the month](#Day-of-the-month)
- [Month](#Month)
- [Day of the week](#Day-of-the-week)
- [Year](#Year)


## Special Characters
- `*` means all possible values.
- `,` means a list of values.
- `-` means a range of values.
- `/` means a step of values.

> Example:
>
> `2,5,7` means `2`, `5` and `7`.
>
> `2-5` means `2`, `3`, `4` and `5`.
>
> `*/2` means every 2 values. (`0`, `2`, `4`, ...)


## Seconds
- The seconds part is optional.
- If you don't specify the seconds part, the default value is `0`.
- The seconds part is a number between `0` and `59`.


## Minutes
- The minutes part is a number between `0` and `59`.

## Hours
- The hours part is a number between `0` and `23`.

## Day of the Month
- **The value `0` is invalid.**
- The day of the month part is a number between `1` and `31`.
- Leap years are taken into account.
- If the day of the month is greater than the number of days in the month (like February), the cron will not be executed.

## Month
- The month part is a number between `1` and `12`.

## Day of the Week
- The day of the week part is a number between `0` and `6`.
- `0` is Sunday.
- `6` is Saturday.
- `7` will be also interpreted as Sunday.
- you can also use `MON`, `TUE`, `WED`, `THU`, `FRI`, `SAT`, `SUN` as values.

## Year
- The year part is a number between `1970` and `2099`.

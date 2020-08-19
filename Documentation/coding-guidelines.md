# Coding guidelines

## Performance
Whenever you want to use a C++ feature that you are not very familiar with, check whether it's appropriate for 
embedded software e.g. it doesn't cost more compared to C alternative.

## Macros
Avoid macros whenever there are alternatives available

### Constants
Instead of using:
```
#define CONSTANT 5
```
use:
```
constexpr int CONSTANT = 5;
```
or if you have multiple constants that are related use enum:
```
enum class ConstantsOfSomething
{
  CONST_1 = 5;
  CONST_2 = 4;
  CONST_3 = 3;
}
```

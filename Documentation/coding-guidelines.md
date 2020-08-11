# Coding guidelines

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

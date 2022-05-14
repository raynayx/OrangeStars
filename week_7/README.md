## R.2
```c
#define MIN(a,b) ((a)<(b) ? (a) : (b))
```

## R.6
- In C, if a variable local to a function is marked as static, it means that variable's state is preserved between the function's calls.

- If a function is marked as static, it means the function is only visible in the translation unit it is defined in. It implies that only other functions in that translation unit can call it.

- A global variable that is marked static is accessible by all functions in that translation unit but not by functions outside that translation unit.

## R.9
```c
a |= (1 << 3) ; //set bit 3
a &= ~(1 << 3); //clear bit 3

```

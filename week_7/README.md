## R.2
```c
#define MINN(a,b) ((a)<(b) ? (a) : (b))
```

## R.6
- In C, if a variable local to a function is marked as startic, it means that variable's state js preserved between the function's calls.

- If a function is marked as static, it means the function is only visie in the translation unit it is defined in. It impliess that only other finctions inn that translation unit can call it.

- Aglobal variable that is marked static is accessible by all functions in that translation unit but not by functions outside that translation unit.

## R.9
```c
a |= (1 << 3) ; //set bit 3
a &= ~(1 << 3); //clear bit 3

```
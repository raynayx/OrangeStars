
# Number of Set bits in `uint32_t` number


## 1. Count Number of Set bits
```C
#include <stdint.h>
#include <stdio.h>

uint8_t bitSet(uint32_t val,uint8_t bitPos)
{
    return (((val & (1 << bitPos)) == (1 << bitPos))? 1 : 0);
}
uint8_t countSetBits(uint32_t val)
{
    uint8_t num_set_bits = 0;
    for(int i=0; i< 32; i++)
    {
        if(bitSet(val,i))
            ++num_set_bits;
    }
    return num_set_bits;
}

int main()
{
    uint32_t data = 0xAAAA;

    printf("Number of set bits: %d\n",countSetBits(data));

    return 0;
}
```

### Comments on 1
With this implementation, all 32 bits will have to be tested in a brute force fashion. It will always required going through all 32 in best and worst case scenarios. If speed isn't utmost importance, this suffices.

## 2. Brian Kernighan's Algorithm 
```C
#include <stdint.h>
#include <stdio.h>


uint8_t countSetBits(uint32_t n)
{
    uint8_t count = 0;
    while(n)
    {
       if(n)
       {
           n &= (n-1);
           ++count;
       }  
    }
    return count; 
}

int main()
{
    printf("%d\n",countSetBits(0xAA));
    return 0;
}

```
### Comments on Brian Kernighan's approach
In this approach,1 is subtracted from the `uint32_t` number in question. The result is then `AND`ed with the origical number. If the result is 0, 0 is return. Else, the bit count is incremented and the process is repeated until 0 is return.
With this approach, the number of iterations is always equal to the number of set bits in the `uint32_t`  number in question.
It generates a shorter assembly compared to the method in 1.

## 3. Using a Lookup Table
```C

#include <stdint.h>
#include <stdio.h>

uint8_t nib_0 = 0x0f;


uint8_t lt[16] = {
    0,1,1,2,1,2,     /*0,1,2,3,4,5*/
    2,3,1,2,2,3,    /*6,7,8,9,10,11*/
    2,3,3,4        /*12,13,14,15*/
};

uint8_t countSetBit(uint32_t value)
{
    uint8_t nibble;

    if(value == 0)
        return lt[0];

    nibble = value & 0x0f; 

    return lt[nibble] + countSetBit(value >> 4);
}

int main()
{
    printf("%d",countSetBit(0xaaaa));
    return 0;
}

```

### Comments on the lookup table approach
With this appraoch, there's a lookup table with 16 values stored. These values are precalculated bit counts in a nibble(4 bits).
The countSetBit function is then called recursively while right shifting the number in question 4 times. The function returns when the argument becomes 0. In each invocation, the lookup table is supplied with 4 bits and it returns number of set bits. These are then added together arrive at the set bit count.
This approach requires storing 16 bytes and then doing as many as 4 right shift operations and additons.


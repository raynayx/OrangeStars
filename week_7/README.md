<!-- Each chapter in the book ends with an interview question followed by the analysis of what an
interviewer would look for in an answer. Choose two of the following questions to answer.
● I.3: What is an ISR?
● I.5: Why do we use the keyword volatile? (Also R.8)
● R.6. What are the uses of the keyword static?
● R.2 Write the ‘standard’ MIN macro. That is, a macro that takes two arguments and
returns the smaller of the two arguments.
● I.12: Is it possible for a variable to be both volatile and const?
● I.15: What is a reentrant function?
● I.16: What kind of loop is better: Count up from zero or count down to zero? Why?
● R.9: Given an integer variable a, write two code fragments. The first should set bit 3 of a.
The second should clear bit 3 of a. In both cases, the other bits should be unmodified.
● R.10: On a certain project it is required to set an integer variable at the absolute address
0x67a9 to the value 0xaa55. Write code to accomplish this task.
● I.20: What are the reasons for segmentation faults in embedded C? How do you avoid
these errors?
● I.31: When does a memory leak occur? What are the ways of avoiding it?
● R.14: What are the problems with dynamic memory allocation in embedded systems?
Answers for the I.x ones are here, answers for the R.x ones are here.
Note: this is intended to be a relatively short assignment to give you time to work on your final
project.
Please note in Discord which questions you chose and how close to a correct answer you got
(right train of thought, syntax, completely different). Be ready to talk about your question in
discussion in Live Class. -->



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

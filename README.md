# **Operating System First Task**

First Task of the Operating System 1 Course from UFRGS

## **Getting Started**

**Debugging**
```
env LGA_LOGGER=LGA_LOGGER_OPTIONS
```

**Test logs**
```
env LGA_LOGGER_TEST=LGA_LOGGER_TEST_OPTIONS
```

**Important logs**
```
env LGA_LOGGER_IMPORTANT=LGA_LOGGER_IMPORTANT_OPTIONS
```
Go to **testes/** directory and run **make "your-test"**, then go to **build/** and run the executable file with the environment variables that you want.


## **LGA_LOGGER_OPTIONS**
* LGA_UNABLE  = 0   - None logs will shown.
* LGA_ERROR  = 1    
* LGA_WARNING  = 2  
* LGA_LOG  = 3      
* LGA_DEBUG = 4     

**EXPLANATION OF EVERY DEBUG LEVEL**
```
LGA_UNABLE - None logs will show up.

LGA_ERROR - Only errors logs will show up. Errors are things that mustn't
happen.

LGA_WARNING - Only errors and warnings will show up. Warning are things that
can happen, but it's a situation that the function is not being used properly.

LGA_LOG - Only errors, warning and logs will show up. Logs explains the
flow of the program.

LGA_DEBUG - Everything will be show up. Debug shows every minor details about the
flow of every function, may have an excess of logs on screen.
```
## **LGA_LOGGER_TEST_OPTIONS**
* LGA_UNABLE  = 0
* LGA_ENABLE  = 1

## **LGA_LOGGER_IMPORTANT_OPTIONS**
* LGA_UNABLE  = 0
* LGA_ENABLE  = 1


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

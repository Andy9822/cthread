# **Operating System First Task**

First Task of the Operating System 1 Course from UFRGS

## **Getting Started**

**Debugging**
```
env LGA_LOGGER=LGA_LOGGER_OPTIONS
```

**Testing**
```
env LGA_LOGGER_TEST_STATUS=LGA_LOGGER_TEST_OPTIONS

The LGA_LOGGER must be at least LGA_LOGGER_TEST to work
```
Go to **testes/** directory and run **make "your-test"**, then go to **build/** and run the executable file with the environment variables that you want.


## **LGA_LOGGER_OPTIONS**
* LGA_UNABLE  = 0
* LGA_ERROR  = 1
* LGA_WARNING  = 2
* LGA_TEST  = 3
* LGA_LOG  = 4

## **LGA_LOGGER_TEST_OPTIONS**
* LGA_UNABLE  = 0
* LGA_ENABLE  = 1


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

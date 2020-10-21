#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "cmocka.h"

#ifdef _WIN32
/* Compatibility with the Windows standard C library. */
#define vsnprintf _vsnprintf
#endif /* _WIN32 */

#if defined(scanf) || defined(printf) || defined(fprintf)
#error "Not handled yet: redefinition of stdio functions"
#endif

int __real_main();

#define array_length(x) (sizeof(x) / sizeof((x)[0]))

//extern int example_main(int argc, char *argv[]);
//
//int example_test_fprintf(FILE* file, const char *format, ...) CMOCKA_PRINTF_ATTRIBUTE(2, 3);
//int __wrap_printf(const char *format, ...) CMOCKA_PRINTF_ATTRIBUTE(1, 2);

static char temporary_buffer[256];
static char temporary_buffer_stdout[256];
static char temporary_buffer_stderr[256];
static FILE *__test_stdin_file;

/* A mock fprintf function that checks the value of strings printed to the
 * standard error stream or output stream. */
static void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

int __wrap_fprintf(FILE* const file, const char *format, ...) {
    int return_value;
    va_list args;
    assert_true(file == stdout || file == stderr);
    va_start(args, format);
    return_value = vsnprintf(temporary_buffer, sizeof(temporary_buffer),
                             format, args);

    remove_spaces(temporary_buffer);

    if (file == stdout) {
        strcpy(temporary_buffer_stdout, temporary_buffer);
        check_expected_ptr(temporary_buffer_stdout);
    }
    else {
        strcpy(temporary_buffer_stderr, temporary_buffer);
        check_expected_ptr(temporary_buffer_stderr);
    }

    va_end(args);
    return return_value;
}

/* A mock printf function that checks the value of strings printed to the
 * standard output stream. */
int __wrap_printf(const char *format, ...) {
    int return_value;
    va_list args;
    va_start(args, format);
    return_value = vsnprintf(temporary_buffer, sizeof(temporary_buffer),
                             format, args);
    remove_spaces(temporary_buffer);
    check_expected_ptr(temporary_buffer);
    va_end(args);
    return return_value;
}

int __wrap_scanf(const char *format, ...) {
    int return_value;
    va_list args;
    va_start(args, format);

    return_value = vfscanf(__test_stdin_file, format, args);
    va_end(args);
    return return_value;

    }

int __wrap___isoc99_scanf(const char *restrict format, ...) {
     void *args = __builtin_apply_args();
     void *ret = __builtin_apply((void (*)())__wrap_scanf, args, 1000);
     __builtin_return(ret);
}

static void test_example_main_no_args(void **state) {
    const char *args[] = {
            "example",
    };

    (void) state; /* unused */

    assert_int_equal(__real_main(array_length(args), (char **) args), -1);
}

static void test_example_main_many_args(void **state) {
    const char *args[] = {
            "example", "--from=3", "--to=3", "something bad"
    };

    (void) state; /* unused */

    assert_int_equal(__real_main(array_length(args), (char **) args), -2);
}

static void test_main_1(void **state) {
    __test_stdin_file = NULL;
    if ((__test_stdin_file = fopen("data/test1.txt", "r")) == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    const char *args[] = {
            "example", "--from=3",
    };

    (void) state; /* unused */

    expect_string(__wrap_fprintf, temporary_buffer_stdout, "2");
    expect_string(__wrap_fprintf, temporary_buffer_stdout, "1");
    //expect_string(__wrap_printf, temporary_buffer, "3");
    assert_int_equal(__real_main(array_length(args), args), 3);
    if (__test_stdin_file != NULL) fclose(__test_stdin_file);
}

static void test_main_2(void **state){
    __test_stdin_file = NULL;
    if ((__test_stdin_file = fopen("data/test2.txt", "r")) == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    const char *args[] = {
            "example", "--to=9", "--from=3"
    };

    (void) state; /* unused */

    expect_string(__wrap_fprintf, temporary_buffer_stderr, "10");
    expect_string(__wrap_fprintf, temporary_buffer_stdout, "1");

    assert_int_equal(__real_main(array_length(args), (char **) args), 3);
    if (__test_stdin_file != NULL) fclose(__test_stdin_file);
}

int __wrap_main()
{
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_example_main_no_args),
            cmocka_unit_test(test_example_main_many_args),
            cmocka_unit_test(test_main_1),
            cmocka_unit_test(test_main_2),
    };

    if (__test_stdin_file != NULL) fclose(__test_stdin_file);
    return cmocka_run_group_tests(tests, NULL, NULL);
}

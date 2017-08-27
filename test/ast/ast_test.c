#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "../../src/c_src/ast/seq_tree.h"


/* A test case that instantiate a node and check if it is not null */
static void not_null_test_success(void **state) {
	seq_node * root = new_seq_node(NULL, NULL);
	assert_non_null(root);
	(void) state; /* unused */
}



int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(not_null_test_success),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

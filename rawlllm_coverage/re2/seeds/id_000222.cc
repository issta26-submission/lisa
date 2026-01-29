#include <cre2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Configure options: set never_nl, then query some option values.
    cre2_opt_set_never_nl(opt, 1);
    int literal_flag = cre2_opt_literal(opt);
    (void)literal_flag; // deliberately inspect but not use further

    cre2_encoding_t enc = cre2_opt_encoding(opt);
    (void)enc; // inspect encoding

    // Create a set with the options. Use a default anchor (0) cast to cre2_anchor_t.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a few simple patterns to the set.
    int r1 = cre2_set_add_simple(set, "abc");
    int r2 = cre2_set_add_simple(set, "def.*ghi");
    int r3 = cre2_set_add_simple(set, ""); // test empty pattern handling

    // Attempt to compile the set (ignore compile result for this test harness).
    int compile_res = cre2_set_compile(set);
    (void)compile_res;
    (void)r1; (void)r2; (void)r3;

    // Clean up.
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}
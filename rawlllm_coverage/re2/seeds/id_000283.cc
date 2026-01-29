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
//<ID> 283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set a memory limit (uses the required API)
    cre2_opt_set_max_mem(opt, (int64_t)(1 << 20)); // 1 MiB

    // Optionally set literal mode and read it back (use required getter)
    cre2_opt_set_literal(opt, 1);
    int literal = cre2_opt_literal(opt);
    (void)literal; // silence unused-variable warnings

    // Create a set with the options (anchor value provided as 0)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern to the set (required API)
    int add_ret = cre2_set_add_simple(set, "abc");
    (void)add_ret;

    // Optionally compile the set before use
    cre2_set_compile(set);

    // Prepare input text and match storage for consume()
    const char *input = "abcXYZ";
    cre2_string_t text;
    text.data = input;
    text.length = 6; // known length of "abcXYZ"

    // Prepare match storage (at least one slot)
    cre2_string_t matches[1];
    matches[0].data = NULL;
    matches[0].length = 0;

    // Use cre2_consume (required API). Pattern provided as a C string.
    int consume_ret = cre2_consume("abc", &text, matches, 1);
    (void)consume_ret;

    // Clean up
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}
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
//<ID> 307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set one_line flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_one_line(opt, 1);

    // Create a set using the options. Anchor value 0 is used (cast to cre2_anchor_t).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a couple of simple patterns to the set
    int add_r1 = cre2_set_add_simple(set, "foo");
    (void)add_r1;
    int add_r2 = cre2_set_add_simple(set, "bar");
    (void)add_r2;

    // Prepare input text for cre2_consume
    const char *input_cstr = "foobar";
    cre2_string_t text;
    text.data = input_cstr;
    text.length = 6; // length of "foobar"

    // Prepare matches array (up to 3 matches)
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Try to consume with a simple pattern; request 1 match slot
    int consume_r = cre2_consume("foo", &text, matches, 1);
    (void)consume_r;

    // Clean up the set and options
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}
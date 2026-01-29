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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Prepare options and configure them
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        // Set encoding to UTF-8 and limit memory usage
        cre2_opt_set_encoding(opt, CRE2_UTF8);
        cre2_opt_set_max_mem(opt, (int64_t)16 * 1024 * 1024); // 16 MB
    }

    // Example pattern and create a compiled regexp using the options
    const char *pattern = "a(b)c";
    int pattern_len = 5; // length of "a(b)c"
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Query number of capturing groups if compilation succeeded
    int num_groups = 0;
    if (re) {
        num_groups = cre2_num_capturing_groups(re);
        // Destroy the compiled regexp when done
        cre2_delete(re);
        re = nullptr;
    }

    // Prepare subject string and match container for cre2_consume
    cre2_string_t subject = { "abc", 3 };
    cre2_string_t match = { nullptr, 0 };

    // Use cre2_consume with the pattern (uncompiled form), subject and match.
    // Request up to 2 matches (full match + one capturing group).
    int consume_ret = cre2_consume(pattern, &subject, &match, 2);

    // Clean up options
    if (opt) {
        cre2_opt_delete(opt);
        opt = nullptr;
    }

    // Use the results in a trivial way to avoid unused-variable warnings
    (void)num_groups;
    (void)consume_ret;
    (void)match.data;
    (void)match.length;
    (void)subject.data;
    (void)subject.length;

    return 66;
}
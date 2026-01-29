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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable longest-match mode
    cre2_opt_set_longest_match(opt, 1);

    // Set and read back max memory (demonstrate use of cre2_opt_max_mem)
    cre2_opt_set_max_mem(opt, (int64_t) (1024 * 1024)); // 1MB
    auto maxmem = cre2_opt_max_mem(opt);

    // Create a regexp using the options
    const char pattern[] = "^(ab)(c)$";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Options can be deleted after creating the regexp
    cre2_opt_delete(opt);

    // Prepare subject and match storage
    const char text[] = "abc";
    cre2_string_t subject;
    subject.data = text;
    subject.length = (int)(sizeof(text) - 1);

    // Reserve room for full match + 2 capturing groups
    const int nmatch = 3;
    cre2_string_t matches[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    // Attempt a full match using the compiled regexp
    int matched = cre2_full_match_re(re, &subject, matches, nmatch);

    // Clean up regexp
    cre2_delete(re);

    // Silence unused-variable warnings (we intentionally don't inspect results further)
    (void)maxmem;
    (void)matched;

    // Indicate success
    return 66;
}
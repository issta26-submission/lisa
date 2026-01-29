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
//<ID> 2234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo|bar|baz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "here is foo and there is baz and maybe bar";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    struct cre2_set * set = NULL;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int match[8];
    size_t match_len = (size_t)(sizeof(match) / sizeof(match[0]));
    size_t matches_found = 0;
    int is_literal = 0;
    int64_t maxmem = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_max_mem(opt, (int64_t)(16 * 1024 * 1024)); /* 16MB */
    is_literal = cre2_opt_literal(opt);
    maxmem = cre2_opt_max_mem(opt);

    // step 3: Initialize and compile a set
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Operate (match against text using the compiled set)
    memset(match, 0, sizeof(match));
    matches_found = cre2_set_match(set, text_buf, text_len, match, match_len);

    // step 5: Validate (produce a checksum tying together results)
    checksum = (long)pattern_len + (long)text_len + (long)is_literal + (long)maxmem
             + (long)set_add_ret + (long)set_compile_ret + (long)matches_found + (long)match[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "ab([0-9]+)cd";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "start ab123cd end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int match_ret = 0;
    int replace_ret = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    size_t set_match_count = 0;
    int set_match_indices[4];
    int errcode = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Create and populate a set
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    set_add_ret = cre2_set_add_simple(set, "ab[0-9]+");
    set_compile_ret = cre2_set_compile(set);

    // step 4: Operate - prepare buffers, perform match and global replace, and set match
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = "X$1Y";
    rewrite.length = (int)(sizeof("X$1Y") - 1);
    memset(matches, 0, sizeof(matches));
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    set_match_count = cre2_set_match(set, text_buf, text_len, set_match_indices, (size_t)4);

    // step 5: Validate / compute lightweight checksum
    errcode = cre2_error_code(re);
    checksum = (long)pattern_len + (long)match_ret + (long)replace_ret + (long)errcode + (long)set_add_ret + (long)set_compile_ret + (long)text_and_target.length + (long)rewrite.length + (long)matches[0].length + (long)set_match_count;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
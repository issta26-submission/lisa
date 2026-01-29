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
//<ID> 1151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo123bar and foo45bar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "X";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int ret_match = 0;
    int ret_replace = 0;
    int errcode = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, "foo[0-9]+bar");

    // step 3: Compile and prepare buffers
    re = cre2_new(pattern, pattern_len, opt);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    memset(matches, 0, sizeof(matches));

    // step 4: Operate - match and global replace using compiled regexp
    ret_match = cre2_match(re, text_and_target.data, text_and_target.length, 0, text_and_target.length, CRE2_UNANCHORED, matches, nmatch);
    ret_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate - inspect error code and compute lightweight checksum
    errcode = cre2_error_code(re);
    computed_check = (long)ret_match + (long)ret_replace + (long)errcode + (long)matches[0].length + (long)text_and_target.length + (long)rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 976
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alice123 bob456 alice789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[128];
    char rewrite_buf2[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t match;
    cre2_string_t tt2;
    cre2_string_t rw2;
    int easy_res = 0;
    int gr_res_re = 0;
    int gr_res = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(rewrite_buf2, 0, sizeof(rewrite_buf2));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = 0;
    match.data = NULL;
    match.length = 0;
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 2);
    gr_res_re = cre2_global_replace_re(re, &text_and_target, &rewrite);
    tt2.data = "z 1 2 3";
    tt2.length = (int)(sizeof("z 1 2 3") - 1);
    rw2.data = rewrite_buf2;
    rw2.length = 0;
    gr_res = cre2_global_replace(pattern, &tt2, &rw2);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_res + (long)gr_res_re + (long)gr_res + (long)match.length + (long)rewrite.length + (long)rw2.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
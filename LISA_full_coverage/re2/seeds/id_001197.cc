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
//<ID> 1197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf1[] = "prefix foo123 suffix";
    const int text_len1 = (int)(sizeof(text_buf1) - 1);
    char text_buf2[] = "foo1 foo22 foo333 end";
    const int text_len2 = (int)(sizeof(text_buf2) - 1);
    char replace_buf1[] = "replaced";
    const int replace_len1 = (int)(sizeof(replace_buf1) - 1);
    char replace_buf2[] = "$1-$2";
    const int replace_len2 = (int)(sizeof(replace_buf2) - 1);
    char target_buf1[128];
    char target_buf2[256];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text1;
    cre2_string_t text2;
    cre2_string_t text_and_target1;
    cre2_string_t text_and_target2;
    cre2_string_t rewrite1;
    cre2_string_t rewrite2;
    cre2_string_t pmatches[4];
    cre2_string_t cmatches[4];
    int partial_ret = 0;
    int consume_ret = 0;
    int replace_ret = 0;
    int global_replace_ret = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);
    memset(pmatches, 0, sizeof(pmatches));
    memset(cmatches, 0, sizeof(cmatches));
    memset(target_buf1, 0, sizeof(target_buf1));
    memset(target_buf2, 0, sizeof(target_buf2));
    text1.data = text_buf1;
    text1.length = text_len1;
    text2.data = text_buf2;
    text2.length = text_len2;
    text_and_target1.data = target_buf1;
    text_and_target1.length = 0;
    text_and_target2.data = target_buf2;
    text_and_target2.length = 0;
    rewrite1.data = replace_buf1;
    rewrite1.length = replace_len1;
    rewrite2.data = replace_buf2;
    rewrite2.length = replace_len2;
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    partial_ret = cre2_partial_match(pattern, &text1, pmatches, (int)(sizeof(pmatches) / sizeof(pmatches[0])));
    consume_ret = cre2_consume(pattern, &text2, cmatches, (int)(sizeof(cmatches) / sizeof(cmatches[0])));
    replace_ret = cre2_replace_re(re, &text_and_target1, &rewrite1);
    global_replace_ret = cre2_global_replace_re(re, &text_and_target2, &rewrite2);

    // step 4: Validate / compute lightweight checksum
    err_code = cre2_error_code(re);
    checksum = (long)partial_ret + (long)consume_ret + (long)replace_ret + (long)global_replace_ret
               + (long)pmatches[0].length + (long)cmatches[0].length + (long)text1.length + (long)text2.length
               + (long)text_and_target1.length + (long)text_and_target2.length + (long)rewrite1.length + (long)rewrite2.length
               + (long)pattern_len + (long)err_code;
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}
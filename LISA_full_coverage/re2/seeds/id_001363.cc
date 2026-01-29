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
//<ID> 1363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(.+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxfooxxxbarzz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf1[] = "[$1]";
    const int rewrite_len1 = (int)(sizeof(rewrite_buf1) - 1);
    char rewrite_buf2[] = "REPLACED";
    const int rewrite_len2 = (int)(sizeof(rewrite_buf2) - 1);
    char target_buf[128];
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_range_t ranges[4];
    cre2_string_t strings[4];
    int add_ret = 0;
    int replace_ret1 = 0;
    int replace_ret2 = 0;
    int prog_size = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target_str, 0, sizeof(target_str));
    memset(ranges, 0, sizeof(ranges));
    memset(strings, 0, sizeof(strings));
    memset(target_buf, 0, sizeof(target_buf));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add_simple(set, pattern);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    text_str.data = text_buf;
    text_str.length = text_len;
    rewrite_str.data = rewrite_buf1;
    rewrite_str.length = rewrite_len1;
    target_str.data = target_buf;
    target_str.length = (int)sizeof(target_buf);
    replace_ret1 = cre2_replace_re(re, &text_str, &rewrite_str);
    rewrite_str.data = rewrite_buf2;
    rewrite_str.length = rewrite_len2;
    replace_ret2 = cre2_global_replace_re(re, &text_str, &rewrite_str);
    strings[0].data = text_buf;
    strings[0].length = text_len;
    strings[1].data = target_buf;
    strings[1].length = target_str.length;
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)text_len + (long)add_ret + (long)replace_ret1 + (long)replace_ret2
                     + (long)prog_size + (long)ranges[0].start + (long)ranges[0].past + (long)strings[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxfoobaryyfoo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "${1}";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[128];
    memset(target_buf, 0, sizeof(target_buf));
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int set_add_ret = 0;
    int program_sz = 0;
    int replace_ret = 0;
    int global_replace_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target_str, 0, sizeof(target_str));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    set_add_ret = cre2_set_add_simple(set, pattern);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    program_sz = cre2_program_size(re);
    text_str.data = text_buf;
    text_str.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    target_str.data = target_buf;
    target_str.length = (int)sizeof(target_buf);
    replace_ret = cre2_replace_re(re, &text_str, &rewrite_str);
    global_replace_ret = cre2_global_replace_re(re, &text_str, &rewrite_str);

    // step 4: Additional transformation with strings_to_ranges
    strings[0].data = "foo";
    strings[0].length = 3;
    strings[1].data = "bar";
    strings[1].length = 3;
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)program_sz
                     + (long)replace_ret + (long)global_replace_ret + (long)set_add_ret
                     + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start
                     + (long)ranges[1].past + (long)(re != NULL) + (long)(set != NULL)
                     + (long)text_str.length + (long)rewrite_str.length + (long)target_str.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
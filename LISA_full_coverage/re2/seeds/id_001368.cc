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
//<ID> 1368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxfooyyfoozz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "bar";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t strings_arr[2];
    cre2_range_t ranges[2];
    int set_add_ret = 0;
    int replace_ret = 0;
    int global_replace_ret = 0;
    int prog_size = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(strings_arr, 0, sizeof(strings_arr));
    memset(ranges, 0, sizeof(ranges));
    memset(target_buf, 0, sizeof(target_buf));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    set_add_ret = cre2_set_add_simple(set, pattern);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    strings_arr[0] = text_and_target;
    strings_arr[1] = rewrite_str;
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite_str);
    global_replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite_str);
    cre2_strings_to_ranges(text_buf, ranges, strings_arr, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)rewrite_len
                     + (long)set_add_ret + (long)replace_ret + (long)global_replace_ret
                     + (long)prog_size + (long)(re != NULL) + (long)(set != NULL)
                     + (long)strings_arr[0].length + (long)strings_arr[1].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
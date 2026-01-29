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
//<ID> 1365
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
    char text_buf[] = "aaa foo bbb foo ccc";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "X";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t strings[4];
    cre2_range_t ranges[4];
    int add_ret = 0;
    int compile_ret = 0;
    int replace_ret = 0;
    int global_replace_ret = 0;
    int prog_size = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();
    /* cre2_set_new expects a cre2_anchor_t for the second argument; cast 0 to that type */
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite_str);
    global_replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite_str);
    strings[0].data = text_buf + 4;
    strings[0].length = 3;
    strings[1].data = text_buf + 12;
    strings[1].length = 3;
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)text_len + (long)rewrite_len
                     + (long)add_ret + (long)compile_ret + (long)replace_ret + (long)global_replace_ret
                     + (long)prog_size + (long)ranges[0].start + (long)ranges[1].start;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
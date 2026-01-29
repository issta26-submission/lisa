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
//<ID> 1361
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
    char text_buf[] = "xxfoobaryy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "${1}-X";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int add_simple_ret = 0;
    int compile_ret = 0;
    int program_size = 0;
    int replace_ret = 0;
    int global_replace_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target_str, 0, sizeof(target_str));
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(target_buf, 0, sizeof(target_buf));
    opt = cre2_opt_new();

    // step 3: Configure / Operate
    cre2_opt_set_perl_classes(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_simple_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    program_size = cre2_program_size(re);
    text_str.data = text_buf;
    text_str.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    target_str.data = target_buf;
    target_str.length = (int)sizeof(target_buf);
    replace_ret = cre2_replace_re(re, &text_str, &rewrite_str);
    global_replace_ret = cre2_global_replace_re(re, &text_str, &rewrite_str);
    strings[0].data = text_buf + 2; // point to "foobaryy" -> "foo"
    strings[0].length = 3;
    cre2_strings_to_ranges(text_buf, ranges, strings, 1);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)program_size + (long)add_simple_ret + (long)compile_ret
                     + (long)replace_ret + (long)global_replace_ret
                     + (long)ranges[0].start + (long)ranges[0].past
                     + (long)text_str.length + (long)rewrite_str.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
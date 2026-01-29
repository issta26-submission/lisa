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
//<ID> 1367
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
    char text_buf[] = "xxfoobarzz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite1_buf[] = "${1}";
    const int rewrite1_len = (int)(sizeof(rewrite1_buf) - 1);
    char rewrite2_buf[] = "X";
    const int rewrite2_len = (int)(sizeof(rewrite2_buf) - 1);
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite1_str;
    cre2_string_t rewrite2_str;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int add_ret = 0;
    int prog_size = 0;
    int replace_ret = 0;
    int global_replace_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(target_buf, 0, sizeof(target_buf));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite1_str, 0, sizeof(rewrite1_str));
    memset(&rewrite2_str, 0, sizeof(rewrite2_str));
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    add_ret = cre2_set_add_simple(set, pattern);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    prog_size = cre2_program_size(re);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite1_str.data = rewrite1_buf;
    rewrite1_str.length = rewrite1_len;
    rewrite2_str.data = rewrite2_buf;
    rewrite2_str.length = rewrite2_len;
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite1_str);
    global_replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite2_str);
    strings[0].data = text_buf + 2;
    strings[0].length = 3;
    strings[1].data = text_buf + 5;
    strings[1].length = 3;
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)prog_size + (long)replace_ret
                     + (long)global_replace_ret + (long)add_ret
                     + (long)strings[0].length + (long)strings[1].length
                     + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
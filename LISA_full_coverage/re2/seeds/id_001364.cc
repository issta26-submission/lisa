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
//<ID> 1364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf1[] = "xxfoobaryy";
    const int text_len1 = (int)(sizeof(text_buf1) - 1);
    char text_buf2[] = "zzfoobarsub";
    const int text_len2 = (int)(sizeof(text_buf2) - 1);
    char rewrite_buf[] = "${first}_X";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[128];
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str1;
    cre2_string_t text_str2;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int add_ret = 0;
    int replace_ret1 = 0;
    int replace_ret2 = 0;
    int prog_size = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(target_buf, 0, sizeof(target_buf));
    memset(&text_str1, 0, sizeof(text_str1));
    memset(&text_str2, 0, sizeof(text_str2));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target_str, 0, sizeof(target_str));
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    add_ret = cre2_set_add_simple(set, pattern);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    text_str1.data = text_buf1;
    text_str1.length = text_len1;
    text_str2.data = text_buf2;
    text_str2.length = text_len2;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    target_str.data = target_buf;
    target_str.length = (int)sizeof(target_buf);
    replace_ret1 = cre2_replace_re(re, &text_str1, &rewrite_str);
    replace_ret2 = cre2_global_replace_re(re, &text_str2, &rewrite_str);
    strings[0].data = text_buf1;
    strings[0].length = 3;
    strings[1].data = text_buf1 + 3;
    strings[1].length = 4;
    cre2_strings_to_ranges(text_buf1, ranges, strings, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len1 + (long)text_len2 + (long)rewrite_len
                     + (long)replace_ret1 + (long)replace_ret2 + (long)prog_size
                     + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL)
                     + (long)add_ret + (long)ranges[0].start + (long)ranges[0].past
                     + (long)ranges[1].start + (long)ranges[1].past + (long)target_str.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
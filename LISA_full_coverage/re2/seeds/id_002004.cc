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
//<ID> 2004
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "prefix foo123 middle foo456 suffix";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_range_t ranges[2];
    cre2_string_t strings[2];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    char target_buf[64];
    char rewrite_buf[64];
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(ranges, 0, sizeof(ranges));
    memset(strings, 0, sizeof(strings));
    memset(target_buf, 0, sizeof(target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Operate - compile regexp and prepare strings/ranges and replacement
    re = cre2_new(pattern, pattern_len, opt);
    strings[0].data = "foo123";
    strings[0].length = (int)(sizeof("foo123") - 1);
    strings[1].data = "foo456";
    strings[1].length = (int)(sizeof("foo456") - 1);
    cre2_strings_to_ranges(text, ranges, strings, 2);
    text_and_target.data = text;
    text_and_target.length = text_len;
    rewrite_buf[0] = 'X';
    rewrite_buf[1] = '\0';
    rewrite.data = rewrite_buf;
    rewrite.length = 1;
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Operate - create named groups iterator (usage) and compute checksum
    iter = cre2_named_groups_iter_new(re);
    checksum = (long)pattern_len + (long)text_len + (long)replace_ret + ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past + (long)(re != NULL) + (long)(iter != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
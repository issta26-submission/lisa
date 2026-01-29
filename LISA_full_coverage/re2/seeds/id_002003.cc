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
//<ID> 2003
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char text_buf[] = "hello world hello";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char target_buf[64];
    const char rewrite_buf[] = "hi";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup - create options and compile regexp
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Prepare text and rewrite buffers
    memset(target_buf, 0, sizeof(target_buf));
    memcpy(target_buf, text_buf, (size_t)text_len);
    text_and_target.data = target_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;

    // step 4: Core operations - perform global replace and create named-groups iterator
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    iter = cre2_named_groups_iter_new(re);

    // step 5: Convert selected substrings into ranges and validate results
    strings[0].data = text_and_target.data;
    strings[0].length = 5;               // "hello" or "hi..." depending on replace result
    strings[1].data = text_and_target.data + 6;
    strings[1].length = 5;
    memset(ranges, 0, sizeof(ranges));
    cre2_strings_to_ranges(text_and_target.data, ranges, strings, 2);
    checksum = (long)pattern_len + (long)text_len + (long)replace_ret + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL) + ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    (void)checksum;
    return 66;
}
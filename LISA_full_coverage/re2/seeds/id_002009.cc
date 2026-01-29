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
//<ID> 2009
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "[0-9]+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "abc123def";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "NUM";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t strings[1];
    cre2_range_t ranges[1];
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    strings[0].data = text_and_target.data + 3;
    strings[0].length = 3;
    memset(ranges, 0, sizeof(ranges));

    // step 3: Operate - compile regexp, create iterator, perform global replace, convert strings to ranges
    re = cre2_new(pattern, pattern_len, NULL);
    iter = cre2_named_groups_iter_new(re);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    cre2_strings_to_ranges(text_and_target.data, ranges, strings, 1);

    // step 4: Validate - simple checksum using results
    checksum = (long)pattern_len + (long)text_and_target.length + (long)rewrite.length + ranges[0].start + ranges[0].past + (long)replace_ret + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}
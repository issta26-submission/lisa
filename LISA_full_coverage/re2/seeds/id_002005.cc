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
//<ID> 2005
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char original_text[] = "baaaab";
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const char rewrite_buf[] = "X";
    cre2_range_t ranges[2];
    cre2_string_t strings[2];
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup - allocate and configure options and initialize string structs
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    text_and_target.data = original_text;
    text_and_target.length = (int)(sizeof(original_text) - 1);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    memset(ranges, 0, sizeof(ranges));
    memset(strings, 0, sizeof(strings));

    // step 3: Core operations - compile regex, perform global replace, create named-groups iterator, convert strings to ranges
    re = cre2_new(pattern, pattern_len, opt);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    iter = cre2_named_groups_iter_new(re);
    strings[0].data = text_and_target.data;
    strings[0].length = (text_and_target.length > 0) ? 1 : 0;
    strings[1].data = (text_and_target.length > 1) ? (text_and_target.data + 1) : text_and_target.data;
    strings[1].length = (text_and_target.length > 1) ? 1 : 0;
    cre2_strings_to_ranges(text_and_target.data, ranges, strings, 2);

    // step 4: Validate - produce a checksum to ensure data flowed through APIs
    checksum = (long)pattern_len + (long)text_and_target.length + (long)rewrite.length + (long)replace_ret + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past + (long)(re != NULL) + (long)(opt != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup - free iterator, regexp and options
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
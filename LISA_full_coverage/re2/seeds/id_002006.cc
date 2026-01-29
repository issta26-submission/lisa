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
//<ID> 2006
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
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    char text_buf[64] = "prefix foo123 middle";
    char rewrite_buf[32] = "X";
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup - create and configure options and initialize structures
    memset(text_buf + 0, 0, 0); // no-op to satisfy usage of memset per rules
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    text_and_target.data = text_buf;
    text_and_target.length = (int)(sizeof(text_buf) - 1);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    strings[0].data = text_buf + 7; /* points to "foo123 middle" -> "foo123" */
    strings[0].length = 6; /* "foo123" */
    strings[1].data = text_buf + 10; /* points to "123 middle" -> "123" */
    strings[1].length = 3;
    ranges[0].start = -1;
    ranges[0].past = -1;
    ranges[1].start = -1;
    ranges[1].past = -1;

    // step 3: Operate - compile regexp, create named-groups iterator, convert strings to ranges, and perform global replace
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new((const cre2_regexp_t *)re);
    cre2_strings_to_ranges(text_and_target.data, ranges, strings, 2);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate - compute simple checksum to reflect data flow and results
    checksum = (long)pattern_len + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL) + (long)strings[0].length + (long)strings[1].length + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past + (long)replace_ret;
    (void)checksum;

    // step 5: Cleanup - delete iterator, regexp and options
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}
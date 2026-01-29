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
//<ID> 2008
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
    const char original_text[] = "prefix foo123 middle foo45 suffix";
    const int original_text_len = (int)(sizeof(original_text) - 1);
    const char rewrite_text[] = "[$1]";
    const int rewrite_text_len = (int)(sizeof(rewrite_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup - initialize structures and options
    memset(ranges, 0, sizeof(ranges));
    memset(strings, 0, sizeof(strings));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate - compile regexp and create named-groups iterator
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Core operations - prepare strings, perform global replace, map substrings to ranges
    text_and_target.data = original_text;
    text_and_target.length = original_text_len;
    rewrite.data = rewrite_text;
    rewrite.length = rewrite_text_len;
    strings[0].data = "foo123";
    strings[0].length = (int)(sizeof("foo123") - 1);
    strings[1].data = "foo45";
    strings[1].length = (int)(sizeof("foo45") - 1);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    cre2_strings_to_ranges(text_and_target.data, ranges, strings, 2);

    // step 5: Validate & Cleanup - compute simple checksum and release resources
    checksum = (long)pattern_len + (long)original_text_len + (long)rewrite_text_len + (long)replace_ret + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
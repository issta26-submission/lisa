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
//<ID> 2002
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxfoo123yy";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup - create and configure options and compile regexp
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations - create named groups iterator, map strings to ranges, perform global replace
    iter = cre2_named_groups_iter_new(re);
    strings[0].data = "foo";
    strings[0].length = 3;
    strings[1].data = "123";
    strings[1].length = 3;
    memset(ranges, 0, sizeof(ranges));
    cre2_strings_to_ranges(text, ranges, strings, 2);
    text_and_target.data = text;
    text_and_target.length = text_len;
    rewrite.data = "BAR";
    rewrite.length = 3;
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate - compute a checksum that depends on results
    checksum = (long)pattern_len + (long)text_len + (long)strings[0].length + (long)strings[1].length + (long)ranges[0].start + (long)ranges[0].past + (long)replace_ret + (long)(re != NULL) + (long)(iter != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup - delete iterator, compiled regexp, and options
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
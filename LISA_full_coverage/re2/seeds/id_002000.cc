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
//<ID> 2000
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "prefix foo123 and foo456 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char sub1[] = "foo123";
    const char sub2[] = "foo456";
    const char rewrite_buf[] = "bar$2";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup - initialize containers and options
    memset(ranges, 0, sizeof(ranges));
    memset(strings, 0, sizeof(strings));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // populate string structures pointing to substrings present in text_buf
    strings[0].data = sub1;
    strings[0].length = (int)(sizeof(sub1) - 1);
    strings[1].data = sub2;
    strings[1].length = (int)(sizeof(sub2) - 1);

    // prepare text_and_target and rewrite structures
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    // step 3: Operate - convert strings to ranges within the text
    cre2_strings_to_ranges(text_and_target.data, ranges, strings, 2);

    // step 4: Operate - compile regexp and create named-groups iterator
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new((const cre2_regexp_t *)re);

    // step 5: Operate - perform a global replace using the compiled regexp
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 6: Validate and Cleanup
    checksum = (long)pattern_len + (long)text_len + (long)replace_ret + (long)ranges[0].start + (long)ranges[0].past + (long)(re != NULL) + (long)(iter != NULL) + (long)(opt != NULL);
    (void)checksum;
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
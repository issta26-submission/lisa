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
//<ID> 2007
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
    char buf[] = "Xfoo123Yfoo45Z";
    int buf_len = (int)(sizeof(buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup - create options, compile regexp, create named-groups iterator
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Core operations - prepare strings pointing into the text buffer and convert to ranges
    strings[0].data = buf + 1; strings[0].length = 6;  // "foo123" at offset 1
    strings[1].data = buf + 8; strings[1].length = 5;  // "foo45" at offset 8
    cre2_strings_to_ranges(buf, ranges, strings, 2);

    // step 4: Operate/Validate - perform a global replace using the compiled regexp and compute a checksum
    text_and_target.data = buf;
    text_and_target.length = buf_len;
    rewrite.data = "bar";
    rewrite.length = 3;
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    checksum = (long)pattern_len + (long)buf_len + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past + (long)replace_ret + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL) + (long)(text_and_target.data != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
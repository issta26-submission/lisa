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
//<ID> 2001
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
    const char text[] = "start foo123 end";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const char repl[] = "NUM";
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup - options and compile
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations - prepare strings, convert to ranges, perform global replace
    memset(ranges, 0, sizeof(ranges));
    text_and_target.data = text;
    text_and_target.length = text_len;
    rewrite.data = repl;
    rewrite.length = (int)(sizeof(repl) - 1);
    strings[0].data = text + 6; strings[0].length = 6;
    strings[1].data = text;     strings[1].length = 5;
    cre2_strings_to_ranges(text, ranges, strings, 2);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Inspect named groups iterator and validate results
    iter = cre2_named_groups_iter_new(re);
    checksum = (long)replace_ret + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1565
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)-(bar)-(baz)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char text[] = "foo-bar-baz";
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int add_ret = 0;
    int compile_ret = 0;
    int matches[4];
    size_t match_count = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);
    strings[0].data = text;
    strings[0].length = 3;
    strings[1].data = text + 4;
    strings[1].length = 3;
    strings[2].data = text + 8;
    strings[2].length = 3;

    // step 4: Operate
    cre2_strings_to_ranges(text, ranges, strings, 3);
    match_count = cre2_set_match(set, text, (size_t)(sizeof(text) - 1), matches, 4);

    // step 5: Validate
    checksum = (long)pattern_len + (long)add_ret + (long)compile_ret + (long)enc
               + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past
               + (long)ranges[2].start + (long)ranges[2].past + (long)match_count
               + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}
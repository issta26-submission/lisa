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
//<ID> 1566
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxfoobaryy";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int add_ret = 0;
    int compile_ret = 0;
    int matches_buf[4];
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

    // step 4: Operate
    strings[0].data = text;
    strings[0].length = text_len;
    strings[1].data = text + 2;
    strings[1].length = 6;
    cre2_strings_to_ranges(text, ranges, strings, 2);
    match_count = cre2_set_match(set, text, (size_t)text_len, matches_buf, 4);

    // step 5: Validate
    checksum = (long)pattern_len + (long)enc + (long)add_ret + (long)compile_ret + (long)(re != NULL) + (long)(set != NULL)
               + (long)match_count + ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past
               + (long)strings[0].length + (long)strings[1].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
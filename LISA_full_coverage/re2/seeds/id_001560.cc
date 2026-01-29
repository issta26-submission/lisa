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
//<ID> 1560
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
    cre2_options_t * opt = NULL;
    struct cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char text[] = "hellofoobarworld";
    cre2_string_t strings[2] = {};
    cre2_range_t ranges[2] = {};
    int nmatch = 2;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    (void)cre2_set_add_simple(set, pattern);
    (void)cre2_set_compile(set);

    // step 4: Operate
    strings[0].data = text + 5; strings[0].length = 3;  // "foo"
    strings[1].data = text + 8; strings[1].length = 3;  // "bar"
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)enc + ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past
               + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL) + (long)nmatch;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
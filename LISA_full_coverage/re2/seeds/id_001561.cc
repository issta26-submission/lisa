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
//<ID> 1561
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
    const char text[] = "foobarbaz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    struct cre2_string_t strings[2];
    struct cre2_range_t ranges[2];
    int nmatch = 2;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    strings[0].data = text;
    strings[0].length = 3; // "foo"
    strings[1].data = text + 3;
    strings[1].length = 3; // "bar"
    ranges[0].start = 0;
    ranges[0].past = 0;
    ranges[1].start = 0;
    ranges[1].past = 0;

    // step 4: Core operations
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)enc + (long)ranges[0].start + (long)ranges[0].past
               + (long)ranges[1].start + (long)ranges[1].past + (long)text_len
               + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)nmatch;
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1567
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    const char text[] = "hello-foobar";
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int nmatch = 3;
    int encoding_val = 0;
    int groups = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    encoding_val = (int)cre2_opt_encoding(opt);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    strings[0].data = text + 6; strings[0].length = 3; /* "foo" */
    strings[1].data = text + 9; strings[1].length = 3; /* "bar" */
    strings[2].data = text + 0; strings[2].length = 5; /* "hello" */
    memset(ranges, 0, sizeof(ranges));
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate
    groups = cre2_num_capturing_groups(re);
    checksum = (long)pattern_len + (long)encoding_val + (long)groups
               + (long)strings[0].length + (long)strings[1].length
               + ranges[0].start + ranges[0].past;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
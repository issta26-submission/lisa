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
//<ID> 1331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t min_s;
    cre2_string_t max_s;
    cre2_string_t strings[1];
    cre2_range_t ranges[1];
    int pmr_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&min_s, 0, sizeof(min_s));
    memset(&max_s, 0, sizeof(max_s));
    memset(&strings, 0, sizeof(strings));
    memset(&ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pmr_ret = cre2_possible_match_range(re, &min_s, &max_s, 64);
    strings[0] = min_s;
    cre2_strings_to_ranges(min_s.data, ranges, strings, 1);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)pmr_ret + (long)(re != NULL) + (long)(opt != NULL)
                     + (long)strings[0].length + (long)ranges[0].start + (long)ranges[0].past;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
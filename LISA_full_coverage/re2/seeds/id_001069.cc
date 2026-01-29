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
//<ID> 1069
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(test|demo)[0-9]*";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "test123 demo456 other789 test";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int match_arr[8];
    const size_t match_len = sizeof(match_arr) / sizeof(match_arr[0]);
    size_t set_matches = 0;
    int version_current = 0;
    int opt_case_flag = 0;
    long computed_check = 0;

    // step 2: Setup (initialize options and query version)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    opt_case_flag = cre2_opt_case_sensitive(opt);
    version_current = cre2_version_interface_current();
    memset(match_arr, 0, sizeof(match_arr));

    // step 3: Core operations (compile regex and create/compile set)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate (match the set against text)
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, match_len);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)version_current + (long)opt_case_flag + (long)set_matches + (long)pattern_len + (long)text_len + (long)match_arr[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
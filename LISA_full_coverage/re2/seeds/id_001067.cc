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
//<ID> 1067
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(test|demo)[0-9]+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "test123 demo456 none789 test000";
    const int text_len = (int)(sizeof(text_buf) - 1);
    int match_arr[8];
    const size_t match_len = sizeof(match_arr) / sizeof(match_arr[0]);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    size_t set_matches = 0;
    int ver_cur = 0;
    int case_flag = 0;
    const char * compiled_pat = NULL;
    long computed_check = 0;

    // step 2: Setup (initialize options and buffers)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    memset(match_arr, 0, sizeof(match_arr));

    // step 3: Core operations (compile regexp and build set)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate (match against text and query version/option)
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, match_len);
    ver_cur = cre2_version_interface_current();
    case_flag = cre2_opt_case_sensitive(opt);
    compiled_pat = cre2_pattern(re);

    // step 5: Validate / compute lightweight checksum using queried values
    computed_check = (long)ver_cur + (long)case_flag + (long)set_matches + (long)pattern_len + (long)text_len + (long)(compiled_pat ? compiled_pat[0] : 0) + (long)match_arr[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
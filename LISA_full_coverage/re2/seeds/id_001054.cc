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
//<ID> 1054
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Z][a-z]+)\\s+([0-9]{1,3})";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice 42 Bob 7 Carol 123";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char min_buf[64];
    char max_buf[64];
    int match_arr[8];
    const size_t match_len = sizeof(match_arr) / sizeof(match_arr[0]);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t minstr;
    cre2_string_t maxstr;
    cre2_string_t subject;
    cre2_string_t single_match;
    const char * compiled_pattern = NULL;
    int pmr_res = 0;
    size_t set_matches = 0;
    long computed_check = 0;
    int64_t maxmem_val = 0;

    // step 2: Setup (initialize options and buffers)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t) (4 * 1024 * 1024)); // 4 MB
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));
    memset(match_arr, 0, sizeof(match_arr));

    // step 3: Core operations - compile regex and create/compile a set
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate - query possible match range, set match, and inspect compiled pattern & options
    minstr.data = min_buf;
    minstr.length = 0;
    maxstr.data = max_buf;
    maxstr.length = 0;
    pmr_res = cre2_possible_match_range(re, &minstr, &maxstr, (int)sizeof(min_buf));
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, match_len);
    compiled_pattern = cre2_pattern(re);
    subject.data = text_buf;
    subject.length = text_len;
    single_match.data = min_buf;
    single_match.length = 0;

    // step 5: Validate / compute lightweight checksum using option query cre2_opt_max_mem
    maxmem_val = cre2_opt_max_mem(opt);
    computed_check = (long)pmr_res + (long)set_matches + (long)minstr.length + (long)maxstr.length + (long)pattern_len + (long)subject.length + (long)maxmem_val + (long)(compiled_pattern != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
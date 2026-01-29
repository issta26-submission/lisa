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
//<ID> 1051
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar)[0-9]*";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123 baz bar456 qux";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    const char * pattern_from_re = NULL;
    int em_res = 0;
    int64_t max_mem_val = 0;
    long computed_check = 0;

    // step 2: Setup (initialize options and buffers)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)4 * 1024 * 1024); // set to 4MB
    memset(match_buf, 0, sizeof(match_buf));
    match.data = match_buf;
    match.length = 0;

    // step 3: Core operations - compile regexp
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate - retrieve pattern from compiled regex and perform easy match
    pattern_from_re = cre2_pattern(re);
    em_res = cre2_easy_match(pattern_from_re, pattern_len, text_buf, text_len, &match, 2);

    // step 5: Validate / compute lightweight checksum using option query
    max_mem_val = cre2_opt_max_mem(opt);
    computed_check = (long)em_res + (long)match.length + (long)pattern_len + (long)text_len + (long)max_mem_val;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
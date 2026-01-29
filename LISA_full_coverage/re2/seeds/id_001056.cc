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
//<ID> 1056
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "sample([0-9]+)X?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "sample123X other sample45";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[64];
    cre2_string_t match;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int match_res = 0;
    int64_t max_mem = 0;
    const char * compiled_pat = NULL;
    long computed_check = 0;

    // step 2: Setup (initialize options and buffers)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)1024 * 1024);
    max_mem = cre2_opt_max_mem(opt);
    memset(match_buf, 0, sizeof(match_buf));

    // step 3: Core operations - compile the regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate - query compiled pattern and attempt a match
    compiled_pat = cre2_pattern(re);
    match.data = match_buf;
    match.length = 0;
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 3);

    // step 5: Validate / compute lightweight checksum using option/query results
    computed_check = (long)max_mem + (long)pattern_len + (long)match.length + (long)match_res + (long)compiled_pat[0] + (long)cre2_opt_log_errors(opt);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
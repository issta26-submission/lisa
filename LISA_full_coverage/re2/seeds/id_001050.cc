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
//<ID> 1050
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(test)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "test123 other test456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t match;
    int match_res = 0;
    const char * pattern_from_re = NULL;
    int64_t max_mem_val = 0;
    long computed_check = 0;

    // step 2: Setup (initialize options and buffers)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t) (2 * 1024 * 1024)); // 2 MB
    memset(match_buf, 0, sizeof(match_buf));

    // step 3: Core operations (compile regex and prepare strings)
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    subject.data = text_buf;
    subject.length = text_len;
    match.data = match_buf;
    match.length = 0;

    // step 4: Operate (perform a match using the compiled regexp)
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 1);

    // step 5: Validate / compute lightweight checksum using option query
    max_mem_val = cre2_opt_max_mem(opt);
    computed_check = (long)match_res + (long)match.length + (long)pattern_len + (long)max_mem_val + (long)subject.length;
    (void)computed_check;
    
    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
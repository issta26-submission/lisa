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
//<ID> 1045
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "hello42 hi hello123 bye";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    char pmatch0_buf[64];
    char pmatch1_buf[64];
    cre2_string_t subject;
    cre2_string_t pmatches[2];
    int partial_res = 0;
    int add_res = 0;
    int compile_res = 0;
    int match_arr[8];
    const size_t match_len = sizeof(match_arr) / sizeof(match_arr[0]);
    size_t set_matches = 0;
    int log_err_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0L;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    memset(pmatch0_buf, 0, sizeof(pmatch0_buf));
    memset(pmatch1_buf, 0, sizeof(pmatch1_buf));
    memset(match_arr, 0, sizeof(match_arr));

    // step 3: Prepare cre2_string_t structures and query options
    subject.data = text_buf;
    subject.length = text_len;
    pmatches[0].data = pmatch0_buf;
    pmatches[0].length = 0;
    pmatches[1].data = pmatch1_buf;
    pmatches[1].length = 0;
    log_err_flag = cre2_opt_log_errors(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Core operations - compile regex, create/compile set, run partial match and set match
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);
    partial_res = cre2_partial_match(pattern, &subject, pmatches, 2);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, match_len);

    // step 5: Validate / compute lightweight checksum (use results and lengths meaningfully)
    computed_check = (long)partial_res + (long)set_matches + (long)add_res + (long)compile_res + (long)pmatches[0].length + (long)pmatches[1].length + (long)pattern_len + (long)subject.length + (long)log_err_flag + (long)enc;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1046
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123bar baz foo456bar extra";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match0_buf[64];
    char match1_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t subject;
    cre2_string_t matches[2];
    int match_arr[8];
    const size_t match_len = sizeof(match_arr) / sizeof(match_arr[0]);
    size_t set_matches = 0;
    int partial_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int log_errors_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    memset(match0_buf, 0, sizeof(match0_buf));
    memset(match1_buf, 0, sizeof(match1_buf));
    memset(match_arr, 0, sizeof(match_arr));

    // step 3: Core operations (compile regex and prepare set)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate (partial match using pattern and run set match)
    subject.data = text_buf;
    subject.length = text_len;
    matches[0].data = match0_buf;
    matches[0].length = 0;
    matches[1].data = match1_buf;
    matches[1].length = 0;
    partial_res = cre2_partial_match(pattern, &subject, matches, 2);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, match_len);
    enc = cre2_opt_encoding(opt);
    log_errors_flag = cre2_opt_log_errors(opt);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)partial_res + (long)set_matches + (long)matches[0].length + (long)matches[1].length + (long)pattern_len + (long)subject.length + (long)enc + (long)log_errors_flag + (long)match_arr[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
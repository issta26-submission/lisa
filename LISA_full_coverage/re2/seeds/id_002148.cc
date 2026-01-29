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
//<ID> 2148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice42 extra";
    int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t text = { text_buf, text_len };
    char match_buf[64];
    cre2_string_t match = { match_buf, 0 };
    char partial_buf[64];
    cre2_string_t partial = { partial_buf, 0 };
    cre2_options_t * opt = NULL;
    int easy_ret = 0;
    int partial_ret = 0;
    int literal_flag = 0;
    int case_flag = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);
    literal_flag = cre2_opt_literal(opt);
    case_flag = cre2_opt_case_sensitive(opt);

    // step 3: Operate (perform an easy match)
    memset(match_buf, 0, sizeof(match_buf));
    match.data = match_buf;
    match.length = 0;
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 2);

    // step 4: Operate (perform a partial match using cre2_partial_match)
    memset(partial_buf, 0, sizeof(partial_buf));
    partial.data = partial_buf;
    partial.length = 0;
    partial_ret = cre2_partial_match(pattern, &text, &partial, 2);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)literal_flag + (long)case_flag + (long)match.length + (long)partial.length + (long)(opt != NULL) + (long)text.length;
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
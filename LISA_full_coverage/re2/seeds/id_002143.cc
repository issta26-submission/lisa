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
//<ID> 2143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123bar";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t match_easy[4];
    cre2_string_t match_partial[4];
    cre2_string_t subject = { text_buf, text_len };
    int easy_ret = 0;
    int partial_ret = 0;
    int literal_flag = 0;
    int case_flag = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate (prepare match arrays and perform matches)
    memset(match_easy, 0, sizeof(match_easy));
    memset(match_partial, 0, sizeof(match_partial));
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, match_easy, 3);
    partial_ret = cre2_partial_match(pattern, &subject, match_partial, 3);

    // step 4: Validate (read option getters and combine results)
    literal_flag = cre2_opt_literal(opt);
    case_flag = cre2_opt_case_sensitive(opt);
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)match_easy[0].length + (long)match_partial[0].length + (long)literal_flag + (long)case_flag + (long)(opt != NULL) + (long)text_len;
    (void)checksum;

    // step 5: Cleanup (release resources)
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
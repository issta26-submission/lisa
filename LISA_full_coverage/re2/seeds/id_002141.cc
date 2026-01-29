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
//<ID> 2141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\bTest\\b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "This is a Test example";
    int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf1[64];
    char match_buf2[64];
    memset(match_buf1, 0, sizeof(match_buf1));
    memset(match_buf2, 0, sizeof(match_buf2));
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t match1 = { match_buf1, 0 };
    cre2_string_t match2 = { match_buf2, 0 };
    cre2_options_t * opt = NULL;
    int literal_flag = 0;
    int case_flag = 0;
    int easy_ret = 0;
    int partial_ret = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    literal_flag = cre2_opt_literal(opt);
    case_flag = cre2_opt_case_sensitive(opt);

    // step 3: Operate (perform an easy match and a partial match)
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match1, 1);
    partial_ret = cre2_partial_match(pattern, &text, &match2, 1);

    // step 4: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)match1.length + (long)match2.length + (long)literal_flag + (long)case_flag + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup (release resources)
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
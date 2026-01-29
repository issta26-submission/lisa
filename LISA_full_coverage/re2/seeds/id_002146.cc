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
//<ID> 2146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([a-z]+)\\d*";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "abc123xyz";
    int text_len = (int)(sizeof(text_buf) - 1);
    char match1_buf[64];
    char match2_buf[64];
    cre2_string_t match1 = { match1_buf, 0 };
    cre2_string_t match2 = { match2_buf, 0 };
    cre2_options_t * opt = NULL;
    int easy_ret = 0;
    int partial_ret = 0;
    int lit_flag = 0;
    int case_flag = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    lit_flag = cre2_opt_literal(opt);
    case_flag = cre2_opt_case_sensitive(opt);

    // step 3: Operate (perform easy and partial matches)
    memset(match1_buf, 0, sizeof(match1_buf));
    match1.data = match1_buf;
    match1.length = 0;
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match1, 2);

    memset(match2_buf, 0, sizeof(match2_buf));
    match2.data = match2_buf;
    match2.length = 0;
    // avoid taking address of a temporary: use a named local cre2_string_t for the text
    cre2_string_t text_str;
    text_str.data = text_buf;
    text_str.length = text_len;
    partial_ret = cre2_partial_match(pattern, &text_str, &match2, 2);

    // step 4: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)match1.length + (long)match2.length + (long)lit_flag + (long)case_flag + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup (release resources)
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
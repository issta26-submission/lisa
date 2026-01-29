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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a|ab)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "abctest abc";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t easy_match;
    cre2_string_t partial_match;
    cre2_string_t text_str;
    int easy_res = 0;
    int partial_res = 0;
    int literal_flag = 0;
    int case_flag = 0;

    // step 2: Setup
    memset(&easy_match, 0, sizeof(easy_match));
    memset(&partial_match, 0, sizeof(partial_match));
    memset(&text_str, 0, sizeof(text_str));
    text_str.data = text;
    text_str.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    literal_flag = cre2_opt_literal(opt);
    case_flag = cre2_opt_case_sensitive(opt);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &easy_match, 1);
    partial_res = cre2_partial_match(pattern, &text_str, &partial_match, 1);

    // step 5: Validate
    (void)literal_flag;
    (void)case_flag;
    (void)easy_res;
    (void)partial_res;
    (void)easy_match;
    (void)partial_match;
    (void)text_str;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
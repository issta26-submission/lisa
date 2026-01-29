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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t easy_match_result;
    cre2_string_t partial_matches[16];
    int perl_flag = 0;
    int set_literal_flag = 0;
    int n_captures = 0;
    int nm = 0;
    int easy_res = 0;
    int partial_res = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&easy_match_result, 0, sizeof(easy_match_result));
    memset(partial_matches, 0, sizeof(partial_matches));
    text_str.data = text;
    text_str.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    set_literal_flag = 0;
    cre2_opt_set_literal(opt, set_literal_flag);
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    n_captures = cre2_num_capturing_groups(re);
    nm = n_captures + 1;
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &easy_match_result, nm);
    partial_res = cre2_partial_match(pattern, &text_str, partial_matches, nm);

    // step 5: Validate
    (void)perl_flag;
    (void)set_literal_flag;
    (void)n_captures;
    (void)nm;
    (void)easy_res;
    (void)partial_res;
    (void)text_str;
    (void)easy_match_result;
    (void)partial_matches;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
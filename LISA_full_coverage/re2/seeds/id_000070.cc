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
//<ID> 70
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
    cre2_string_t match_str;
    int perl_classes_flag = 0;
    int groups_count = 0;
    int partial_res = 0;
    int easy_res = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&match_str, 0, sizeof(match_str));
    text_str.data = text;
    text_str.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 0);
    perl_classes_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    groups_count = cre2_num_capturing_groups(re);
    partial_res = cre2_partial_match(pattern, &text_str, &match_str, 3);
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match_str, 3);

    // step 5: Validate
    (void)perl_classes_flag;
    (void)groups_count;
    (void)partial_res;
    (void)easy_res;
    (void)text_str;
    (void)match_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
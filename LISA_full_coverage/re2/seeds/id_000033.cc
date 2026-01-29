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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc(d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xyz abc123 more abc456end";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t *opt = cre2_opt_new();
    cre2_string_t match_easy;
    cre2_string_t match_partial;
    cre2_string_t text_str;
    int easy_res = 0;
    int partial_res = 0;
    int literal_flag = 0;
    int case_flag = 0;

    // step 2: Setup
    memset(&match_easy, 0, sizeof(match_easy));
    memset(&match_partial, 0, sizeof(match_partial));
    text_str.data = text;
    text_str.length = text_len;
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure (read back option flags)
    literal_flag = cre2_opt_literal(opt);
    case_flag = cre2_opt_case_sensitive(opt);

    // step 4: Operate (perform easy match and partial match)
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match_easy, 1);
    partial_res = cre2_partial_match(pattern, &text_str, &match_partial, 1);

    // step 5: Validate (use values to ensure meaningful data flow)
    (void)literal_flag;
    (void)case_flag;
    (void)easy_res;
    (void)partial_res;
    (void)match_easy;
    (void)match_partial;
    (void)text_str;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
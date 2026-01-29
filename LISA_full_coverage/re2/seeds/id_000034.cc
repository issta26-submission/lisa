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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(te)(st)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "testing cre2 partial and easy matches";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t *opt = NULL;
    cre2_string_t match;
    cre2_string_t input;
    cre2_string_t submatches[2];
    int easy_res = 0;
    int partial_res = 0;
    int literal_flag = 0;
    int case_flag = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    memset(&input, 0, sizeof(input));
    memset(submatches, 0, sizeof(submatches));
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure (read back option settings)
    literal_flag = cre2_opt_literal(opt);
    case_flag = cre2_opt_case_sensitive(opt);

    // step 4: Operate (perform easy match and partial match)
    input.data = text;
    input.length = text_len;
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);
    partial_res = cre2_partial_match(pattern, &input, submatches, 2);

    // step 5: Validate (use results to satisfy meaningful data flow)
    (void)literal_flag;
    (void)case_flag;
    (void)easy_res;
    (void)partial_res;
    (void)match;
    (void)submatches;
    (void)input;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
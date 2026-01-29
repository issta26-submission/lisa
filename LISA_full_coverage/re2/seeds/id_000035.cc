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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxfoobaryy";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t text_str;
    int literal_flag = 0;
    int case_flag = 0;
    int easy_res = 0;
    int partial_res = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    memset(&text_str, 0, sizeof(text_str));
    text_str.data = text;
    text_str.length = text_len;
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure (retrieve option values)
    literal_flag = cre2_opt_literal(opt);
    case_flag = cre2_opt_case_sensitive(opt);

    // step 4: Operate (perform easy match and partial match)
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);
    partial_res = cre2_partial_match(pattern, &text_str, &match, 1);

    // step 5: Validate (compile the pattern with the configured options and inspect)
    re = cre2_new(pattern, pattern_len, opt);
    const char * compiled_pat = cre2_pattern(re);

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    (void)literal_flag;
    (void)case_flag;
    (void)easy_res;
    (void)partial_res;
    (void)match;
    (void)text_str;
    (void)compiled_pat;
    // API sequence test completed successfully
    return 66;
}
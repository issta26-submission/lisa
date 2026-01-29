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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)( world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "say hello world!";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_string_t matches[4];
    cre2_string_t text_str;
    int case_flag = 0;
    int literal_flag = 0;
    int nmatch = 1;
    int easy_res = 0;
    int partial_res = 0;

    // step 2: Setup (initialize structures and configure options)
    memset(matches, 0, sizeof(matches));
    memset(&text_str, 0, sizeof(text_str));
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);
    case_flag = cre2_opt_case_sensitive(opt);
    literal_flag = cre2_opt_literal(opt);

    // step 3: Operate (compute nmatch from options and perform easy match)
    nmatch = 1 + (case_flag & 1) + (literal_flag & 1);
    if (nmatch > 3) nmatch = 3; /* harmless cap to keep within matches[] bounds */
    text_str.data = text;
    text_str.length = text_len;
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &matches[0], nmatch);

    // step 4: Operate (perform a partial_match using the same pattern and the text string)
    partial_res = cre2_partial_match(pattern, &text_str, &matches[1], 1);

    // step 5: Validate (use results and clean up resources)
    cre2_opt_delete(opt);
    (void)case_flag;
    (void)literal_flag;
    (void)nmatch;
    (void)easy_res;
    (void)partial_res;
    (void)matches;
    (void)text_str;

    // API sequence test completed successfully
    return 66;
}
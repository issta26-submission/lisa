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
//<ID> 147
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
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$1-$2";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int easy_res = 0;
    int prog_size = 0;
    int replace_res = 0;
    int nmatch = 3;
    int combined = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Operate (easy match, compile, inspect, replace)
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / propagate results (meaningful data flow, no branching)
    combined = easy_res + prog_size + replace_res;
    cre2_opt_set_one_line(opt, combined & 1);
    cre2_opt_set_case_sensitive(opt, (combined >> 1) & 1);
    (void)matches;
    (void)text_and_target;
    (void)rewrite;
    (void)nmatch;
    (void)pattern_len;
    (void)input_text_len;
    (void)easy_res;
    (void)prog_size;
    (void)replace_res;
    (void)combined;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
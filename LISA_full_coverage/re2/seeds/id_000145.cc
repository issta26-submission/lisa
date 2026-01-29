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
//<ID> 145
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
    const int nmatch = 3;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    cre2_string_t rewrite;
    cre2_string_t text_and_target;
    int easy_res = 0;
    int prog_size = 0;
    int replace_res = 0;
    int combined = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&text_and_target, 0, sizeof(text_and_target));
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;

    // step 3: Operate - easy match against the pattern (no compiled regex)
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);

    // step 4: Operate - compile regex, inspect program size, then perform a replace using the compiled regex
    re = cre2_new(pattern, pattern_len, NULL);
    prog_size = cre2_program_size(re);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / propagate results (use results meaningfully without branching)
    combined = easy_res + prog_size + replace_res;
    matches[0].length = matches[0].length + (combined & 0xFF);
    (void)matches;
    (void)rewrite;
    (void)text_and_target;
    (void)easy_res;
    (void)prog_size;
    (void)replace_res;
    (void)combined;
    (void)pattern_len;
    (void)input_text_len;
    (void)nmatch;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}
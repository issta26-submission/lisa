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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(test)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char original_text[] = "this is test and another test";
    const int original_text_len = (int)(sizeof(original_text) - 1);
    const char replacement[] = "REPL";
    const int replacement_len = (int)(sizeof(replacement) - 1);
    char buffer[256];
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;

    // step 2: Setup (initialize buffers and structures)
    memset(&match, 0, sizeof(match));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, original_text, (size_t)original_text_len);
    text_and_target.data = buffer;
    text_and_target.length = original_text_len;
    rewrite.data = replacement;
    rewrite.length = replacement_len;

    // step 3: Configure (set options and compile)
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (easy match, global replace, and verify match after replace)
    int easy_before = cre2_easy_match(pattern, pattern_len, buffer, original_text_len, &match, 1);
    int replace_res = cre2_global_replace(pattern, &text_and_target, &rewrite);
    int easy_after = cre2_easy_match(pattern, pattern_len, text_and_target.data, text_and_target.length, &match, 1);

    // step 5: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    (void)easy_before;
    (void)replace_res;
    (void)easy_after;
    (void)match;
    (void)text_and_target;
    // API sequence test completed successfully
    return 66;
}
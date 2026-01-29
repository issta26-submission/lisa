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
//<ID> 26
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
    const char text[] = "this is test and another test";
    const int text_len = (int)(sizeof(text) - 1);
    const char replacement[] = "REPL";
    const int replacement_len = (int)(sizeof(replacement) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    char buffer[256];

    // step 2: Setup (configure options and prepare buffers)
    memset(&match, 0, sizeof(match));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(buffer, 0, sizeof(buffer));
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    memcpy(buffer, text, (size_t)text_len);
    text_and_target.data = buffer;
    text_and_target.length = text_len;
    rewrite.data = replacement;
    rewrite.length = replacement_len;

    // step 3: Operate (perform an easy match)
    int easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);

    // step 4: Operate (perform a global replace on the mutable buffer)
    int replace_res = cre2_global_replace(pattern, &text_and_target, &rewrite);

    // step 5: Cleanup (release compiled regexp and options)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    (void)easy_res;
    (void)replace_res;
    (void)match;
    (void)text_and_target;
    (void)rewrite;
    return 66;
}
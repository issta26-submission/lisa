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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(hello)(world)?$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_full[] = "helloworld";
    const int text_full_len = (int)(sizeof(text_full) - 1);
    const char text_context[] = "prefix helloworld suffix";
    const int text_context_len = (int)(sizeof(text_context) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t full_matches[3];
    cre2_string_t text_str;
    int prog_size = 0;
    int easy_res = 0;
    int full_res = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    memset(full_matches, 0, sizeof(full_matches));
    memset(&text_str, 0, sizeof(text_str));
    text_str.data = text_full;
    text_str.length = text_full_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    easy_res = cre2_easy_match(pattern, pattern_len, text_context, text_context_len, &match, 2);
    full_res = cre2_full_match(pattern, &text_str, full_matches, 3);

    // step 5: Validate
    (void)prog_size;
    (void)easy_res;
    (void)full_res;
    (void)match;
    (void)full_matches;
    (void)text_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
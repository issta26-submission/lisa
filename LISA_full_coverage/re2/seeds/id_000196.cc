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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>hello)(world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "greeting: helloworld!";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const int nmatch = 3;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match;
    const char * found_name = NULL;
    int found_index = 0;
    bool iter_res = false;
    int easy_res = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    match.data = input_text;
    match.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Core operations (compile, easy match, named-group iteration)
    re = cre2_new(pattern, pattern_len, opt);
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &match, nmatch);
    iter = cre2_named_groups_iter_new(re);
    iter_res = cre2_named_groups_iter_next(iter, &found_name, &found_index);

    // step 4: Validate / propagate results (use values to influence state; no branching)
    match.length = match.length + ((easy_res + (int)iter_res + found_index) & 7);
    cre2_opt_set_one_line(opt, (match.length >> 1) & 1);
    cre2_opt_set_case_sensitive(opt, (easy_res >> 1) & 1);
    cre2_opt_set_perl_classes(opt, (found_name != NULL) & 1);
    (void)pattern_len;
    (void)input_text_len;
    (void)found_name;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}
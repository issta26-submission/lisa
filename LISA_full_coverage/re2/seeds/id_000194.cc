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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix helloworld suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const int nmatch = 3;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match;
    const char * ng_name = NULL;
    int ng_index = -1;
    int easy_res = 0;
    int prog_size = 0;
    int err_code = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &match, nmatch);
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &ng_name, &ng_index);

    // step 4: Inspect results
    prog_size = cre2_program_size(re);
    err_code = cre2_error_code(re);

    // step 5: Validate / propagate results (use values meaningfully; no branching)
    match.length = match.length + ((easy_res + err_code + prog_size) & 7);
    ng_index = ng_index + ((easy_res ^ err_code) & 3);
    (void)ng_name;
    (void)pattern_len;
    (void)input_text_len;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
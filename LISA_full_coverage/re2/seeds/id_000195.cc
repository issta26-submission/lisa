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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "xx foobar yy";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const int nmatch = 3;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[4];
    const char * ng_name = NULL;
    int ng_index = -1;
    int easy_res = 0;
    bool ng_next = false;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations (compile and match)
    re = cre2_new(pattern, pattern_len, opt);
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    iter = cre2_named_groups_iter_new(re);
    ng_next = cre2_named_groups_iter_next(iter, &ng_name, &ng_index);

    // step 4: Inspect / propagate results (use values to influence state; no branching)
    matches[0].length = matches[0].length + ((easy_res + (ng_index & 7)) & 7);
    cre2_opt_set_one_line(opt, (easy_res + ng_index) & 1);
    cre2_opt_set_case_sensitive(opt, (matches[0].length >> 1) & 1);
    (void)ng_next;
    (void)ng_name;
    (void)pattern_len;
    (void)input_text_len;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(Foo)(Bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "xxx FooBar yyy Foo baz";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    cre2_string_t matches[4];
    int set_match_indices[4];
    int nmatch = 4;
    int match_res = 0;
    size_t set_match_count = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&set_match_indices, 0, sizeof(set_match_indices));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_res = cre2_match(re, input_text, input_text_len, 0, input_text_len, CRE2_UNANCHORED, &matches[0], nmatch);
    set_match_count = cre2_set_match(set, input_text, input_text_len, set_match_indices, (size_t)4);

    // step 5: Validate / propagate results (use values to influence state; no branching)
    matches[0].length = matches[0].length + ((match_res + (int)set_match_count) & 7);
    matches[0].data = matches[0].data;
    cre2_opt_set_case_sensitive(opt, (match_res + (int)set_match_count) & 1);
    cre2_opt_set_encoding(opt, (cre2_encoding_t)((match_res + (int)set_match_count) & 2));
    set_match_indices[0] = set_match_indices[0] + (match_res & 3);
    (void)pattern_len;
    (void)input_text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
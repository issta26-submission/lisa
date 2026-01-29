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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(abc)(?:def)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "xx abcdef yy abc zz";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t matches[4];
    int set_match_indices[4];
    int nmatch = 3;
    int match_res = 0;
    size_t set_count = 0;
    int add_res = 0;
    int compile_res = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&set_match_indices, 0, sizeof(set_match_indices));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Operate (compile regexp and perform a match, build a set and match it)
    re = cre2_new(pattern, pattern_len, opt);
    match_res = cre2_match(re, input_text, input_text_len, 0, input_text_len, CRE2_UNANCHORED, &matches[0], nmatch);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);
    set_count = cre2_set_match(set, input_text, (size_t)input_text_len, set_match_indices, (size_t)4);

    // step 5: Validate / propagate results (use values to influence state; no branching)
    matches[0].length = matches[0].length + ((match_res + (int)set_count + add_res + compile_res) & 7);
    matches[0].data = matches[0].data;
    cre2_opt_set_case_sensitive(opt, ((match_res >> 1) ^ (int)set_count) & 1);
    cre2_opt_set_encoding(opt, (set_count & 1) ? CRE2_UTF8 : CRE2_UNKNOWN);
    (void)pattern_len;
    (void)input_text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
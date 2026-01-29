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
//<ID> 180
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
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t matches[4];
    int nmatch = 3;
    int match_res = 0;
    int set_add_res = 0;
    int set_compile_res = 0;
    cre2_encoding_t enc = CRE2_UTF8;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    set_add_res = cre2_set_add_simple(set, pattern);
    set_compile_res = cre2_set_compile(set);

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_res = cre2_match(re, input_text, input_text_len, 0, input_text_len, CRE2_UNANCHORED, &matches[0], nmatch);

    // step 5: Validate / propagate results (use values to influence state; no branching)
    matches[0].length = matches[0].length + ((match_res + set_add_res + set_compile_res) & 7);
    cre2_opt_set_case_sensitive(opt, (match_res >> 1) & 1);
    enc = (cre2_encoding_t)(CRE2_UTF8 + ((match_res + set_add_res) & 1));
    cre2_opt_set_encoding(opt, enc);
    (void)pattern_len;
    (void)input_text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
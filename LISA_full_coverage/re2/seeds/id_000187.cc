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
//<ID> 187
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
    const int nmatch = 4;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t matches[4];
    int set_matches_idx[4];
    char set_err[64];
    int match_res = 0;
    size_t set_res = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&set_matches_idx, 0, sizeof(set_matches_idx));
    memset(&set_err, 0, sizeof(set_err));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Core operations - compile regexp and create a set, add pattern and compile set
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add(set, pattern, (size_t)pattern_len, set_err, sizeof(set_err));
    cre2_set_compile(set);

    // step 4: Operate - perform a single regexp match and a set match against the same text
    match_res = cre2_match(re, input_text, input_text_len, 0, input_text_len, CRE2_UNANCHORED, &matches[0], nmatch);
    set_res = cre2_set_match(set, input_text, (size_t)input_text_len, set_matches_idx, 4);

    // step 5: Validate / propagate results (use results to change option flags; no branching)
    cre2_opt_set_case_sensitive(opt, ((match_res + (int)set_res) >> 1) & 1);
    cre2_opt_set_encoding(opt, (match_res & 1) ? CRE2_UTF8 : CRE2_Latin1);
    matches[0].length = matches[0].length + ((match_res + (int)set_res) & 7);
    set_matches_idx[0] = set_matches_idx[0] + ((int)match_res & 3);
    (void)pattern_len;
    (void)input_text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Prepare input text as literals so we can set lengths without needing strlen.
    const char text_lit[] = "hello world";
    cre2_string_t input = { text_lit, (int)(sizeof(text_lit) - 1) };

    // 1) Use cre2_partial_match with an inline pattern (not a compiled regex).
    // Prepare an array for matches (one capture group).
    cre2_string_t pm_matches[1];
    pm_matches[0].data = NULL;
    pm_matches[0].length = 0;
    // Try a partial match for "hell" inside "hello world".
    int pm_result = cre2_partial_match("hell", &input, pm_matches, 1);
    (void)pm_result; // keep quiet if unused

    // 2) Use cre2_quote_meta to quote a string containing regex metacharacters.
    const char orig_lit[] = "a+b*c?";
    cre2_string_t original = { orig_lit, (int)(sizeof(orig_lit) - 1) };
    cre2_string_t quoted;
    quoted.data = NULL;
    quoted.length = 0;
    int quote_res = cre2_quote_meta(&quoted, &original);
    (void)quote_res;

    // 3) Compile a regex and use cre2_full_match_re to run a full-match with captures.
    const char pat_lit[] = "^(hello) world$";
    cre2_regexp_t * re = cre2_new(pat_lit, (int)(sizeof(pat_lit) - 1), NULL);

    // Prepare storage for the full match and one capturing group.
    cre2_string_t fm_matches[2];
    fm_matches[0].data = NULL;
    fm_matches[0].length = 0;
    fm_matches[1].data = NULL;
    fm_matches[1].length = 0;

    int fm_result = 0;
    if (re) {
        fm_result = cre2_full_match_re(re, &input, fm_matches, 2);
    }
    (void)fm_result;

    // 4) Create a named-groups iterator (if regex compiled) and then delete it.
    cre2_named_groups_iter_t * iter = NULL;
    if (re) {
        iter = cre2_named_groups_iter_new(re);
    }
    // Use the required API to delete the iterator if created.
    if (iter) {
        cre2_named_groups_iter_delete(iter);
        iter = NULL;
    }

    // Clean up compiled regex if created.
    if (re) {
        cre2_delete(re);
        re = NULL;
    }

    return 66;
}
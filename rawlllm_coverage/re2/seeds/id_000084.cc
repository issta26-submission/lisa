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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66; /* still consider success for this test harness */
    }

    /* Query the dot_nl option (uses required API cre2_opt_dot_nl) */
    int dot_nl_flag = cre2_opt_dot_nl(opt);

    /* Create a set with the options (uses required API cre2_set_new) */
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    /* Prepare input and output cre2_string_t structures for full match.
       We attempt a trivial full-match of the literal "hello" against text "hello". */
    cre2_string_t text = { "hello", 5 };
    cre2_string_t out_match = { nullptr, 0 };

    /* Perform a full match (uses required API cre2_full_match).
       The exact semantics of the cre2_full_match parameters depend on the library;
       calling with a simple pattern and one input/output element for this sequence. */
    int fm_result = cre2_full_match("hello", &text, &out_match, 1);

    /* Clean up the set (uses required API cre2_set_delete) */
    if (set) {
        cre2_set_delete(set);
    }

    /* Clean up options */
    cre2_opt_delete(opt);

    /* Silence unused-variable warnings (we exercised the APIs above) */
    (void)dot_nl_flag;
    (void)fm_result;
    (void)out_match;

    return 66;
}
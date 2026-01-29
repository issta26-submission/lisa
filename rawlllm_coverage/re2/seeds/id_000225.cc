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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the never_nl flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }
    cre2_opt_set_never_nl(opt, 1);

    // Prepare an input for cre2_consume
    cre2_string_t input;
    input.data = "abc123def";
    input.length = (int)(sizeof("abc123def") - 1);

    // Prepare a container for the consumed match
    cre2_string_t consumed_match;
    consumed_match.data = nullptr;
    consumed_match.length = 0;

    // Attempt to consume a sequence of digits from the input
    int consume_res = cre2_consume("\\d+", &input, &consumed_match, 0);

    // Prepare strings for cre2_replace
    cre2_string_t text_and_target;
    text_and_target.data = "replace 456 here";
    text_and_target.length = (int)(sizeof("replace 456 here") - 1);

    cre2_string_t rewrite;
    rewrite.data = "NUM";
    rewrite.length = (int)(sizeof("NUM") - 1);

    // Perform a simple replacement of digits with "NUM"
    int replace_res = cre2_replace("\\d+", &text_and_target, &rewrite);

    // Clean up options
    cre2_opt_delete(opt);

    // Use results to avoid unused-variable warnings (no-op)
    (void)consume_res;
    (void)replace_res;
    (void)consumed_match;

    return 66;
}
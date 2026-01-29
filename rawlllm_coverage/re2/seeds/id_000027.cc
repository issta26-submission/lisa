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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options couldn't be created, still cleanly return success code as required.
        return 66;
    }

    // Configure options: treat patterns literally and enable logging of errors.
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    // Prepare text (target) and rewrite strings for cre2_replace.
    char textbuf[64] = "foo bar foo";
    cre2_string_t text_and_target;
    // compute length without relying on strlen
    int tlen = 0;
    while (textbuf[tlen]) ++tlen;
    text_and_target.data = textbuf;
    text_and_target.length = tlen;

    const char *rewrite_s = "baz";
    cre2_string_t rewrite;
    int rlen = 0;
    while (rewrite_s[rlen]) ++rlen;
    rewrite.data = rewrite_s;
    rewrite.length = rlen;

    // Use cre2_replace with a literal pattern "foo"
    const char *pattern = "foo";
    int replace_rc = cre2_replace(pattern, &text_and_target, &rewrite);
    (void)replace_rc; // ignore result for this test sequence

    // Create a regexp object using the same options and then delete it.
    cre2_regexp_t *re = cre2_new(pattern, 3, opt); // "foo" length is 3
    if (re) {
        cre2_delete(re);
    }

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}
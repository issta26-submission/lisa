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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options and set encoding */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    /* Prepare a simple pattern and text for cre2_easy_match */
    const char *pattern = "a(b)c";
    int pattern_len = 5; /* length of "a(b)c" */
    const char *text = "abc";
    int text_len = 3; /* length of "abc" */

    /* Prepare match storage for the full match and one capturing group */
    cre2_string_t matches[2];
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;

    int match_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 2);

    /* Prepare a mutable buffer for global replace (text_and_target).
       Use known initial contents and length to avoid relying on external headers. */
    char buffer[64] = "xabbby"; /* initial content */
    cre2_string_t text_and_target;
    text_and_target.data = buffer;
    text_and_target.length = 6; /* length of "xabbby" */

    /* Replacement string ("z") */
    cre2_string_t rewrite;
    rewrite.data = "z";
    rewrite.length = 1;

    /* Perform a global replace of one-or-more 'b' characters with "z" */
    int greplace_ret = cre2_global_replace("b+", &text_and_target, &rewrite);

    /* Call the named-groups iterator delete function with a null pointer
       (we did not create an iterator in this sequence). */
    cre2_named_groups_iter_delete((cre2_named_groups_iter_t *)0);

    /* Clean up options */
    cre2_opt_delete(opt);

    /* Consider the sequence successful if the cre2 calls did not return negative errors */
    if (match_ret >= 0 && greplace_ret >= 0) return 66;
    return 2;
}
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
//<ID> 218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options and enable POSIX syntax */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_posix_syntax(opt, 1);

    /* Prepare pattern and input as arrays so sizeof can be used to get lengths */
    const char pattern_lit[] = "([0-9]+)-(\\w+)";
    const int pattern_len = (int)(sizeof(pattern_lit) - 1);

    const char input_lit[] = "1234-abcde rest";
    const int input_len = (int)(sizeof(input_lit) - 1);

    /* Compile the regex with the options */
    cre2_regexp_t *re = cre2_new(pattern_lit, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare cre2_string_t for the input text */
    cre2_string_t text;
    text.data = input_lit;
    text.length = input_len;

    /* Prepare a match container (initially empty) */
    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    /* Try to find and consume a match from the input. Request one match slot. */
    int found = cre2_find_and_consume_re(re, &text, &match, 1);

    /* Prepare rewrite and target for extract operation regardless of find outcome */
    const char rewrite_lit[] = "$2:$1";
    cre2_string_t rewrite;
    rewrite.data = rewrite_lit;
    rewrite.length = (int)(sizeof(rewrite_lit) - 1);

    cre2_string_t target;
    target.data = NULL;
    target.length = 0;

    /* Call extract_re to demonstrate extraction with the compiled regexp */
    int extracted = cre2_extract_re(re, &text, &rewrite, &target);

    /* Clean up resources */
    cre2_delete(re);
    cre2_opt_delete(opt);

    /* Consider success if we at least called the APIs without crashing; return 66 */
    (void)found;     /* silence unused-variable warnings if any */
    (void)extracted; /* silence unused-variable warnings if any */
    return 66;
}
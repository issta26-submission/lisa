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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options and set a maximum memory budget */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Set max memory via provided setter (use a modest value) */
    cre2_opt_set_max_mem(opt, (int64_t)1024 * 1024); /* 1 MiB */

    /* Query the max memory back (use long long to avoid requiring stdint.h here) */
    long long maxmem = (long long)cre2_opt_max_mem(opt);
    (void)maxmem; /* silence unused-variable warnings */

    /* Query the library interface revision */
    int revision = cre2_version_interface_revision();
    (void)revision;

    /* Create a compiled regexp using the options above.
       Pattern "(ab)c" has length 5 characters. */
    const char *pattern = "(ab)c";
    cre2_regexp_t *rex = cre2_new(pattern, 5, opt);
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare input text for matching and extraction.
       Use the literal "zabcp" (length 5). */
    cre2_string_t text;
    text.data = "zabcp";
    text.length = 5;

    /* Use cre2_partial_match with the pattern string (uncompiled)
       Provide an array for up to 2 match groups (whole match + one capture). */
    cre2_string_t match_array[2];
    match_array[0].data = NULL; match_array[0].length = 0;
    match_array[1].data = NULL; match_array[1].length = 0;
    int partial_ok = cre2_partial_match(pattern, &text, match_array, 2);
    (void)partial_ok;

    /* Prepare a rewrite template and a target for extract_re.
       Use rewrite "X\\1Y" (4 characters: 'X' '\' '1' 'Y'). */
    cre2_string_t rewrite;
    rewrite.data = "X\\1Y";
    rewrite.length = 4;

    cre2_string_t target;
    target.data = NULL;
    target.length = 0;

    /* Perform extract using the compiled regexp */
    int extract_ok = cre2_extract_re(rex, &text, &rewrite, &target);
    (void)extract_ok;

    /* Clean up compiled regexp and options */
    cre2_delete(rex);
    cre2_opt_delete(opt);

    /* If we reached here, the API sequence executed successfully */
    return 66;
}
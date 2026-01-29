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
//<ID> 335
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "ab ab";
    const int textlen = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t matches[4];
    int match_res = 0;
    int fac_res = 0;
    int revision = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const int nmatch = 4;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    input.data = text;
    input.length = textlen;
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    revision = cre2_version_interface_revision();

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_res = cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, matches, nmatch);
    fac_res = cre2_find_and_consume_re(re, &input, matches, nmatch);

    // step 4: Validate
    int combined = match_res + fac_res + (int)enc + revision;
    (void)combined;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 334
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
    const char text[] = "xxfoobarxx";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t groups[4];
    cre2_string_t matches[4];
    int match_ret = 0;
    int consume_ret = 0;
    int rev = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    char probe[4];

    // step 2: Setup
    memset(groups, 0, sizeof(groups));
    memset(matches, 0, sizeof(matches));
    memset(probe, 0, sizeof(probe));
    input.data = text;
    input.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    rev = cre2_version_interface_revision();
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_ret = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
    consume_ret = cre2_find_and_consume_re(re, &input, groups, 3);

    // step 5: Validate
    probe[0] = (char)(match_ret + consume_ret + rev + (int)enc);
    probe[1] = re ? ((const char *)cre2_pattern(re))[0] : 0;
    probe[2] = matches[0].length ? (char)matches[0].length : 0;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    (void)probe;
    (void)groups;
    (void)input;
    return 66;
}
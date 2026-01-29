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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(abc)(def)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char full_text[] = "xxabcdefyy";
    char workbuf[48];
    cre2_string_t text_str;
    cre2_string_t matches[3];
    cre2_string_t single_match;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int version_rev = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int match_res = 0;
    int consume_res = 0;
    int computed = 0;

    // step 2: Setup
    memset(workbuf, 0, sizeof(workbuf));
    memset(matches, 0, sizeof(matches));
    memset(&single_match, 0, sizeof(single_match));
    opt = cre2_opt_new();
    text_str.data = full_text;
    text_str.length = (int)(sizeof(full_text) - 1);

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    version_rev = cre2_version_interface_revision();

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_res = cre2_match(re, full_text, text_str.length, 0, text_str.length, CRE2_UNANCHORED, matches, 3);
    single_match.data = NULL;
    single_match.length = 0;
    consume_res = cre2_find_and_consume_re(re, &text_str, &single_match, 1);

    // step 5: Validate
    computed = (int)(version_rev + (int)enc + match_res + consume_res + matches[0].length + text_str.length);
    workbuf[0] = (char)(computed & 0xFF);

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    (void)workbuf;
    (void)computed;
    return 66;
}
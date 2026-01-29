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
//<ID> 337
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(he)(ll)o";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int rev = 0;
    cre2_string_t matches[3];
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    int match_res = 0;
    int find_consume_res = 0;
    int combined = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    text_str.data = text;
    text_str.length = text_len;
    rewrite_str.data = "";
    rewrite_str.length = 0;
    opt = cre2_opt_new();
    rev = cre2_version_interface_revision();

    // step 3: Configure
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_res = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
    find_consume_res = cre2_find_and_consume_re(re, &text_str, &rewrite_str, rev + (int)enc);

    // step 5: Validate
    combined = match_res + find_consume_res + (matches[0].length);

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    (void)combined;
    return 66;
}
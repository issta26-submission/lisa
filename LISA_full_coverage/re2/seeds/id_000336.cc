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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(cd)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxabcdyy";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    cre2_string_t consumed;
    cre2_string_t input;
    int match_result = 0;
    int find_consume_result = 0;
    int version_rev = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&consumed, 0, sizeof(consumed));
    memset(&input, 0, sizeof(input));
    opt = cre2_opt_new();

    // step 3: Configure / Inspect
    version_rev = cre2_version_interface_revision();
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_result = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
    input.data = text;
    input.length = text_len;
    find_consume_result = cre2_find_and_consume_re(re, &input, &consumed, 3);

    // step 5: Validate
    checksum = match_result + find_consume_result + version_rev + (int)enc + matches[0].length + matches[1].length + matches[2].length + consumed.length + input.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
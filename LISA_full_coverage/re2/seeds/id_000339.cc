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
//<ID> 339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello) (world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char input_buf[] = "hello world and more";
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text;
    cre2_string_t matches[3];
    cre2_string_t consume_target;
    int match_result = 0;
    int consume_result = 0;
    int version_rev = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(target_buf, 0, sizeof(target_buf));
    memset(matches, 0, sizeof(matches));
    text.data = input_buf;
    text.length = (int)(sizeof(input_buf) - 1);
    consume_target.data = target_buf;
    consume_target.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    (void)enc;

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_result = cre2_match(re, input_buf, (int)(sizeof(input_buf) - 1), 0, text.length, CRE2_UNANCHORED, matches, 3);
    consume_result = cre2_find_and_consume_re(re, &text, &consume_target, 0);

    // step 5: Validate
    version_rev = cre2_version_interface_revision();
    computed_check = match_result + consume_result + version_rev + (int)cre2_opt_encoding(opt) + matches[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
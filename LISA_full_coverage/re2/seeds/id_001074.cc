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
//<ID> 1074
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab|cd)[0-9]+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "ab123 cd456 other ab789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int posix_flag = 0;
    int matched = 0;
    long computed_check = 0;

    // step 2: Setup (create options and query encoding)
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    enc = cre2_opt_encoding(opt);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 3: Core operations (compile regex, create named groups iterator, perform match)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    memset(matches, 0, sizeof(matches));
    matched = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)matched + (long)pattern_len + (long)text_len + (long)enc + (long)posix_flag + (long)(matches[0].length);
    (void)computed_check;

    // step 5: Cleanup named iterator and regex/options
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalization
    // API sequence test completed successfully
    return 66;
}
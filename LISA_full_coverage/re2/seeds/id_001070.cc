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
//<ID> 1070
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?[0-9]+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foobar123 hello foo456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match[4];
    int match_ret = 0;
    int posix_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup (initialize options and configure encoding/posix)
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    memset(match, 0, sizeof(match));

    // step 3: Core operations (compile regex and create named-groups iterator)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (perform a match and query option state)
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match, 4);
    posix_flag = cre2_opt_posix_syntax(opt);
    enc = cre2_opt_encoding(opt);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)match_ret + (long)pattern_len + (long)text_len + (long)posix_flag + (long)match[0].length + (long)((int)enc);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
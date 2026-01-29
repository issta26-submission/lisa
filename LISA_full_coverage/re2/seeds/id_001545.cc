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
//<ID> 1545
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text;
    cre2_string_t matches[3];
    char textbuf[] = "xxfooxx";
    int match_ret = 0;
    int named_index = -1;
    int log_flag = 0;
    int literal_flag = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(matches, 0, sizeof(matches));
    text.data = textbuf;
    text.length = (int)(sizeof(textbuf) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_log_errors(opt, 1);
    log_flag = cre2_opt_log_errors(opt);
    literal_flag = cre2_opt_literal(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    match_ret = cre2_match(re, text.data, text.length, 0, text.length, CRE2_UNANCHORED, &matches[0], 3);
    named_index = cre2_find_named_capturing_groups(re, "name");
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_delete(iter);

    // step 5: Validate (compute checksum to use values)
    checksum = (long)pattern_len + (long)match_ret + (long)named_index + (long)log_flag + (long)literal_flag
               + (long)(re != NULL) + (long)text.length + (long)matches[0].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
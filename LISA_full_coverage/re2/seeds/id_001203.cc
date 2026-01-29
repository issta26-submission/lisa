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
//<ID> 1203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(user):([0-9]+)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char partial_pattern[] = "user([0-9]+)";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    char text_buf[] = "user42 extra";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1-UID:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t input_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t partial_matches[4];
    int partial_ret = 0;
    int extract_ret = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    memset(&input_str, 0, sizeof(input_str));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target_str, 0, sizeof(target_str));
    memset(partial_matches, 0, sizeof(partial_matches));
    input_str.data = text_buf;
    input_str.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    target_str.data = NULL;
    target_str.length = 0;
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    partial_ret = cre2_partial_match(partial_pattern, &input_str, partial_matches, 2);
    extract_ret = cre2_extract_re(re, &input_str, &rewrite_str, &target_str);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)partial_pattern_len + (long)text_len
                     + (long)partial_ret + (long)extract_ret
                     + (long)partial_matches[0].length + (long)target_str.length
                     + (long)enc + (long)(re != NULL) + (long)(iter != NULL);
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}
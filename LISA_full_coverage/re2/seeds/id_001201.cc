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
//<ID> 1201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char partial_pattern[] = "foo([0-9]+)";
    const int partial_pattern_unused_len = 0; (void)partial_pattern_unused_len;
    char text_buf[] = "prefix foo123 and bar45 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t input;
    cre2_string_t rewrite_str;
    cre2_string_t target;
    cre2_string_t partial_matches[4];
    int partial_ret = 0;
    int extract_ret = 0;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    memset(&input, 0, sizeof(input));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target, 0, sizeof(target));
    memset(partial_matches, 0, sizeof(partial_matches));
    input.data = text_buf;
    input.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;

    // step 3: Compile / Configure
    re = cre2_new(pattern, pattern_len, opt);
    encoding = cre2_opt_encoding(opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    partial_ret = cre2_partial_match(partial_pattern, &input, partial_matches, 3);
    extract_ret = cre2_extract_re(re, &input, &rewrite_str, &target);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)encoding + (long)partial_ret + (long)extract_ret
                     + (long)input.length + (long)rewrite_str.length + (long)target.length
                     + (long)partial_matches[0].length + (long)(re != NULL) + (long)(iter != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
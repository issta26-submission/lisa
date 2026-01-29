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
//<ID> 930
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "prefix\\s*([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "prefix 12345 rest of text";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t partial_matches[3];
    cre2_string_t input;
    int set_indices[4];
    int add_res = 0;
    size_t set_count = 0;
    int partial_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, "12345|67890");
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    memset(partial_matches, 0, sizeof(partial_matches));
    input.data = text_buf;
    input.length = text_len;
    memset(set_indices, -1, sizeof(set_indices));
    partial_res = cre2_partial_match(pattern, &input, partial_matches, 2);
    set_count = cre2_set_match(set, text_buf, (size_t)text_len, set_indices, 4);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)enc + (long)partial_res + (long)add_res + (long)set_count + (long)partial_matches[0].length + (long)set_indices[0] + (long)pattern_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
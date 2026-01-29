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
//<ID> 1100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\b([A-Za-z]+)([0-9]*)\\b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alpha123 beta gamma456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_obj;
    cre2_string_t text_obj2;
    cre2_string_t matches1[4];
    cre2_string_t matches2[4];
    const int nmatch = (int)(sizeof(matches1) / sizeof(matches1[0]));
    int word_boundary_flag = 0;
    int partial_ret = 0;
    int find_ret = 0;
    const char * pat_from_re = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    word_boundary_flag = cre2_opt_word_boundary(opt);
    memset(matches1, 0, sizeof(matches1));
    memset(matches2, 0, sizeof(matches2));

    // step 3: Compile / Create regexp
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);

    // step 4: Core operations
    text_obj.data = text_buf;
    text_obj.length = text_len;
    partial_ret = cre2_partial_match(pattern, &text_obj, matches1, nmatch);
    text_obj2.data = text_buf;
    text_obj2.length = text_len;
    find_ret = cre2_find_and_consume(pat_from_re, &text_obj2, matches2, nmatch);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)word_boundary_flag + (long)partial_ret + (long)find_ret + (long)pattern_len + (long)text_len + (long)(matches1[0].length) + (long)(matches2[0].length);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
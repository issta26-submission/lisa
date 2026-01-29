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
//<ID> 1771
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello|world";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "hello world";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[64];
    memset(text_buf, 0, sizeof(text_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int match_indices[8];
    memset(match_indices, 0, sizeof(match_indices));
    int set_add_res = 0;
    int set_compile_res = 0;
    size_t set_match_count = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    memcpy(text_buf, source_text, (size_t)source_text_len);
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Prepare strings and ranges
    strings[0].data = text_buf;
    strings[0].length = 5; /* "hello" */
    strings[1].data = text_buf + 6;
    strings[1].length = 5; /* "world" */
    ranges[0].start = 0;
    ranges[0].past = 0;
    ranges[1].start = 0;
    ranges[1].past = 0;

    // step 4: Operate (convert strings to ranges, build set, and match)
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);
    set = cre2_set_new(opt, (cre2_anchor_t)CRE2_UNANCHORED);
    set_add_res = cre2_set_add_simple(set, pattern);
    set_compile_res = cre2_set_compile(set);
    set_match_count = cre2_set_match(set, text_buf, (size_t)source_text_len, match_indices, (size_t)8);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)enc + (long)set_add_res + (long)set_compile_res + (long)set_match_count + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL) + (long)match_indices[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
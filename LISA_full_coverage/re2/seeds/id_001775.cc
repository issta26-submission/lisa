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
//<ID> 1775
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "xxfoobarlyy foobar zz";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int match_indices[8];
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(match_indices, 0, sizeof(match_indices));
    int nmatch = 3;
    int match_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    size_t set_match_count = 0;
    long checksum = 0;
    int set_add_res = 0;
    int set_compile_res = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    match_res = cre2_match(re, source_text, source_text_len, 0, source_text_len, (cre2_anchor_t)CRE2_UNANCHORED, strings, nmatch);
    cre2_strings_to_ranges(source_text, ranges, strings, nmatch);
    set = cre2_set_new(opt, (cre2_anchor_t)CRE2_UNANCHORED);
    set_add_res = cre2_set_add_simple(set, pattern);
    set_compile_res = cre2_set_compile(set);
    set_match_count = cre2_set_match(set, source_text, (size_t)source_text_len, match_indices, (size_t)8);

    // step 4: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)match_res + (long)enc + (long)set_add_res + (long)set_compile_res + (long)set_match_count + (long)strings[0].length + (long)strings[1].length + (long)strings[2].length + (long)ranges[0].start + (long)ranges[0].past;
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
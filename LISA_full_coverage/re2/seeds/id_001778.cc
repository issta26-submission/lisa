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
//<ID> 1778
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
    const char source_text[] = "foo bar foobar";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t strings[4];
    cre2_range_t ranges[4];
    int match_indices[8];
    cre2_encoding_t enc = CRE2_UNKNOWN;
    size_t set_matches = 0;
    long checksum = 0;
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(match_indices, 0, sizeof(match_indices));

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    strings[0].data = source_text;
    strings[0].length = 3; /* "foo" */
    strings[1].data = source_text + 4;
    strings[1].length = 3; /* "bar" */
    /* prepare set */
    set = cre2_set_new(opt, (cre2_anchor_t)CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    cre2_strings_to_ranges(source_text, ranges, strings, 2);
    set_matches = cre2_set_match(set, source_text, (size_t)source_text_len, match_indices, (size_t)8);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)enc + (long)set_matches + (long)ranges[0].start + (long)ranges[0].past + (long)match_indices[0] + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
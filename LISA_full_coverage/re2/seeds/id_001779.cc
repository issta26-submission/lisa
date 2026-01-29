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
//<ID> 1779
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)|(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "prefooMIDbarPOST";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char work_buf[64];
    memset(work_buf, 0, sizeof(work_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int match_indices[8];
    memset(match_indices, 0, sizeof(match_indices));
    memset(ranges, 0, sizeof(ranges));
    memset(strings, 0, sizeof(strings));
    cre2_encoding_t enc = CRE2_UNKNOWN;
    size_t set_match_count = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    set = cre2_set_new(opt, (cre2_anchor_t)CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    memcpy(work_buf, source_text, (size_t)source_text_len);

    // step 4: Operate
    strings[0].data = work_buf;
    strings[0].length = 3;                     // "pre"
    strings[1].data = work_buf + 3;
    strings[1].length = source_text_len - 3;   // "fooMIDbarPOST"
    cre2_strings_to_ranges(work_buf, ranges, strings, 2);
    set_match_count = cre2_set_match(set, work_buf, (size_t)source_text_len, match_indices, (size_t)8);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)enc + (long)set_match_count + (long)match_indices[0] + (long)ranges[0].start + (long)ranges[0].past + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
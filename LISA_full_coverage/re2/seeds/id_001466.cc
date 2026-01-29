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
//<ID> 1466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char raw_pattern[] = "a.*(b)?[c]+";
    const int raw_pattern_len = (int)(sizeof(raw_pattern) - 1);
    const char sample_text[] = "foo bar baz";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t original;
    cre2_string_t quoted;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    const int nmatch = 3;
    int quote_ret = 0;
    const char * used_pattern = NULL;
    int used_pattern_len = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&original, 0, sizeof(original));
    memset(&quoted, 0, sizeof(quoted));
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    original.data = raw_pattern;
    original.length = raw_pattern_len;
    quote_ret = cre2_quote_meta(&quoted, &original);
    used_pattern = quoted.data ? quoted.data : original.data;
    used_pattern_len = quoted.length ? quoted.length : original.length;
    re = cre2_new(used_pattern, used_pattern_len, opt);
    strings[0].data = sample_text;
    strings[0].length = 3;
    strings[1].data = sample_text + 4;
    strings[1].length = 3;
    strings[2].data = sample_text + 8;
    strings[2].length = 3;
    cre2_strings_to_ranges(sample_text, ranges, strings, nmatch);

    // step 5: Validate
    checksum = (long)raw_pattern_len + (long)used_pattern_len + (long)quote_ret
               + (long)(re != NULL) + ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past
               + ranges[2].start + ranges[2].past + (long)sample_text_len;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
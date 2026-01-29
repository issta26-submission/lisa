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
//<ID> 1463
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
    char text_buf[] = "prefix a+b(c)? and more a+b(c)? suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char original_buf[] = "a+b(c)?";
    const int original_len = (int)(sizeof(original_buf) - 1);
    char quoted_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_string_t original;
    cre2_string_t quoted;
    int quote_ret = 0;
    int num_caps = 0;
    long checksum = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    original.data = original_buf;
    original.length = original_len;
    quoted.data = quoted_buf;
    quoted.length = (int)(sizeof(quoted_buf) - 1);
    quote_ret = cre2_quote_meta(&quoted, &original);
    strings[0].data = text_buf;
    strings[0].length = text_len;
    strings[1].data = quoted.data;
    strings[1].length = quoted.length;
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);
    num_caps = cre2_num_capturing_groups(re);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)quoted.length + (long)quote_ret
               + (long)num_caps + (long)ranges[0].start + (long)ranges[0].past
               + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
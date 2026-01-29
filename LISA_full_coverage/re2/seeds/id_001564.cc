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
//<ID> 1564
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
    const char text[] = "hello world";
    const int nmatch = 2;
    cre2_options_t * opt = NULL;
    struct cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int add_ret = 0;
    int compile_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 4: Operate
    strings[0].data = text;
    strings[0].length = 5; /* "hello" */
    strings[1].data = text + 6;
    strings[1].length = 5; /* "world" */
    memset(ranges, 0, sizeof(ranges));
    cre2_strings_to_ranges(text, ranges, strings, nmatch);
    enc = cre2_opt_encoding(opt);

    // step 5: Validate
    checksum = (long)pattern_len + (long)add_ret + (long)compile_ret + (long)enc
               + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past
               + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
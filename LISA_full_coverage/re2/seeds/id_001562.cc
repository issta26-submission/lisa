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
//<ID> 1562
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)|(world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    const char text[] = "hello world";
    cre2_range_t ranges[2];
    cre2_string_t strings[2];
    int nmatch = 2;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Prepare string slices that point into text
    strings[0].data = text;
    strings[0].length = 5;       // "hello"
    strings[1].data = text + 6;
    strings[1].length = 5;       // "world"

    // step 4: Operate - convert the strings into ranges within text
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate (compute a simple checksum using results)
    checksum = (long)pattern_len
               + (long)enc
               + (long)(re != NULL)
               + (long)(set != NULL)
               + (long)strings[0].length
               + (long)strings[1].length
               + (long)ranges[0].start + (long)ranges[0].past
               + (long)ranges[1].start + (long)ranges[1].past;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
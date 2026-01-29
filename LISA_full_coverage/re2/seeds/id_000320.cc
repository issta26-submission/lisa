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
//<ID> 320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char text_buf[] = "hello world example";
    const int nmatch = 3;
    cre2_options_t * opt = NULL;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    long sum_ranges = 0;
    char adjust_char = 0;

    // step 2: Setup
    memset(ranges, 0, sizeof(ranges));
    strings[0].data = text_buf;
    strings[0].length = 5;           // "hello"
    strings[1].data = text_buf + 6;
    strings[1].length = 5;           // "world"
    strings[2].data = text_buf + 12;
    strings[2].length = 7;           // "example"
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_perl_classes(opt, 1);

    // step 4: Operate
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);

    // step 5: Validate
    sum_ranges = ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past + ranges[2].start + ranges[2].past;
    adjust_char = (char)((unsigned char)strings[0].data[0] + (unsigned char)(sum_ranges & 0xFF));
    (void)adjust_char;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
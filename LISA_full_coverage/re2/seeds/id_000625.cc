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
//<ID> 625
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char text_buf[] = "hello_world_test";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int nmatch = 3;
    int case_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);

    // step 3: Configure
    case_flag = cre2_opt_case_sensitive(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    strings[0].data = text_buf;
    strings[0].length = 5;               // "hello"
    strings[1].data = text_buf + 6;
    strings[1].length = 5;               // "world"
    strings[2].data = text_buf + 12;
    strings[2].length = 4;               // "test"
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);

    // step 5: Validate
    computed_check = case_flag + (int)enc
                   + (int)ranges[0].start + (int)ranges[0].past
                   + (int)ranges[1].start + (int)ranges[1].past
                   + (int)ranges[2].start + (int)ranges[2].past
                   + strings[0].length + strings[1].length + strings[2].length
                   + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
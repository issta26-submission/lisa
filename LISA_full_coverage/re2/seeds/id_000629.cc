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
//<ID> 629
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char text_buf[] = "one-two-three";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const int nmatch = 3;
    cre2_options_t * opt = NULL;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int cs_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int len0 = 0;
    int len1 = 0;
    int len2 = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    strings[0].data = text_buf;
    strings[0].length = 3;
    strings[1].data = text_buf + 4;
    strings[1].length = 3;
    strings[2].data = text_buf + 8;
    strings[2].length = 5;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    cs_flag = cre2_opt_case_sensitive(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);

    // step 5: Validate
    len0 = (int)(ranges[0].past - ranges[0].start);
    len1 = (int)(ranges[1].past - ranges[1].start);
    len2 = (int)(ranges[2].past - ranges[2].start);
    computed_check = cs_flag + (int)enc + len0 + len1 + len2 + strings[0].length + strings[1].length + strings[2].length + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
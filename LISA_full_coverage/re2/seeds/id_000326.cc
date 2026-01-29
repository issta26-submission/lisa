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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char sample_text[] = "hello_world_sample";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int nmatch = 3;
    int perl_flag = 1;
    cre2_encoding_t encoding = CRE2_UTF8;
    int computed = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    strings[0].data = text_buf;
    strings[0].length = sample_text_len;
    strings[1].data = text_buf;
    strings[1].length = 5;               // "hello"
    strings[2].data = text_buf + 6;
    strings[2].length = 5;               // "world"
    memset(ranges, 0, sizeof(ranges));

    // step 3: Initialize / Configure
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, perl_flag);
    cre2_opt_set_encoding(opt, encoding);

    // step 4: Operate
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);

    // step 5: Validate / use results
    computed = (int)(ranges[0].start + ranges[1].past + ranges[2].start);
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned char)(computed & 0xFF));

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char sample_text[] = "hello world";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    char result_buf[8];
    int nmatch = 2;
    cre2_encoding_t enc_read;

    // step 2: Setup
    memset(ranges, 0, sizeof(ranges));
    memset(strings, 0, sizeof(strings));
    memset(result_buf, 0, sizeof(result_buf));
    strings[0].data = sample_text + 0;
    strings[0].length = 5; // "hello"
    strings[1].data = sample_text + 6;
    strings[1].length = 5; // "world"

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate / Validate
    cre2_strings_to_ranges(sample_text, ranges, strings, nmatch);
    result_buf[0] = (char)(ranges[0].start & 0xFF);
    result_buf[1] = (char)(ranges[0].past & 0xFF);
    enc_read = cre2_opt_encoding(opt);
    result_buf[2] = (char)((int)enc_read & 0xFF);

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    char text[] = "hello world";
    int nmatch = 2;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_options_t * opt = NULL;
    int perl_flag = 1;
    int checksum = 0;
    char outbuf[8];

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(outbuf, 0, sizeof(outbuf));
    strings[0].data = text;
    strings[0].length = 5;
    strings[1].data = text + 6;
    strings[1].length = 5;

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_perl_classes(opt, perl_flag);

    // step 4: Operate
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate / Use results
    checksum = (int)(ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past);
    outbuf[0] = (char)(checksum & 0xFF);
    text[0] = outbuf[0];

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
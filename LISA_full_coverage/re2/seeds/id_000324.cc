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
//<ID> 324
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
    const int nmatch = 3;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    char workbuf[16];

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(workbuf, 0, sizeof(workbuf));
    strings[0].data = sample_text + 0; strings[0].length = 5;  // "hello"
    strings[1].data = sample_text + 6; strings[1].length = 5;  // "world"
    strings[2].data = sample_text + 3; strings[2].length = 4;  // "lo w"

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    cre2_strings_to_ranges(sample_text, ranges, strings, nmatch);

    // step 5: Validate
    workbuf[0] = (char)( (unsigned)(ranges[0].start & 0xff) + (unsigned)(ranges[1].past & 0xff) + (unsigned)(strings[2].length & 0xff) );
    workbuf[1] = (char)( (unsigned)sample_text_len & 0xff );

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
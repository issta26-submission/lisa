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
//<ID> 240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "item-123 extra";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char match_buf[64];
    cre2_regexp_t * re = NULL;
    cre2_string_t match_s;
    const char * compiled_pat = NULL;
    int ver_age = 0;
    int easy_res = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(match_buf, 0, sizeof(match_buf));
    memset(&match_s, 0, sizeof(match_s));
    match_s.data = match_buf;
    match_s.length = 0;

    // step 3: Core operations
    ver_age = cre2_version_interface_age();
    re = cre2_new(pattern, pattern_len, (const cre2_options_t *)NULL);
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &match_s, nmatch);
    compiled_pat = cre2_pattern(re);

    // step 4: Inspect results
    match_s.length = match_s.length + ((easy_res & 0x3) + (ver_age & 0x7) + ((int)((unsigned long)compiled_pat & 0xFF) & 0xF));

    // step 5: Validate / propagate results
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)match_s.length & 0x7));
    (void)input_text_len;
    (void)compiled_pat;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}
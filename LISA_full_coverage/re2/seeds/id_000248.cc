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
//<ID> 248
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
    const char sample_text[] = "item-123 extra";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char input_buf[64];
    char match_buf[64];
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t match_s;
    const char * compiled_pat = NULL;
    int ver_age = 0;
    int easy_res = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(input_buf, 0, sizeof(input_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&match_s, 0, sizeof(match_s));
    memcpy(input_buf, sample_text, (size_t)sample_text_len);
    text_s.data = input_buf;
    text_s.length = sample_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    re = cre2_new(pattern, pattern_len, NULL);

    // step 3: Core operations
    easy_res = cre2_easy_match(pattern, pattern_len, text_s.data, text_s.length, &match_s, nmatch);
    ver_age = cre2_version_interface_age();
    compiled_pat = cre2_pattern(re);

    // step 4: Inspect results
    match_s.length = match_s.length + ((easy_res + ver_age) & 0xF);
    int cp0 = compiled_pat ? (int)compiled_pat[0] : 0;
    text_s.length = text_s.length + ((match_s.length ^ cp0) & 0x7);

    // step 5: Validate / propagate results
    input_buf[0] = (char)((unsigned char)input_buf[0] + ((unsigned)text_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)match_s.length & 0x7));
    (void)cp0;
    (void)nmatch;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}
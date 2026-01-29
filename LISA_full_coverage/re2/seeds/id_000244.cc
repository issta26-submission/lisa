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
//<ID> 244
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
    char text_buf[64];
    char match_buf[64];
    cre2_regexp_t * re = NULL;
    cre2_string_t match_s;
    const char * compiled_pat = NULL;
    int age = 0;
    int res_easy = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&match_s, 0, sizeof(match_s));
    memcpy(text_buf, input_text, (size_t)input_text_len);
    match_s.data = match_buf;
    match_s.length = 0;
    re = cre2_new(pattern, pattern_len, NULL);

    // step 3: Core operations
    age = cre2_version_interface_age();
    res_easy = cre2_easy_match(pattern, pattern_len, text_buf, input_text_len, &match_s, nmatch);
    compiled_pat = cre2_pattern(re);

    // step 4: Inspect results
    match_s.length = match_s.length + ((res_easy + age + ((compiled_pat != NULL) ? (int)compiled_pat[0] : 0)) & 0xF);
    int propagated = (int)text_buf[0] + ((match_s.length ^ ((compiled_pat != NULL) ? (int)compiled_pat[1] : 0)) & 0x7);
    (void)propagated;

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)match_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)age & 0x7));
    (void)res_easy;
    (void)pattern_len;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}
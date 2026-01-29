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
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_src[] = "bar-42 trailing text";
    const int text_src_len = (int)(sizeof(text_src) - 1);
    char text_buf[128];
    char match_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_s;
    const int nmatch = 2;
    int match_res = 0;
    int rev = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memcpy(text_buf, text_src, (size_t)text_src_len);
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_never_nl(opt, 1);

    // step 3: Create / Configure regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    match_res = cre2_easy_match(pattern, pattern_len, text_buf, text_src_len, &match_s, nmatch);
    rev = cre2_version_interface_revision();

    // step 5: Inspect / propagate results
    match_s.length = match_s.length + ((match_res & 7) + (rev & 3));
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)match_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)text_buf[0] & 0x7));
    (void)match_res;
    (void)rev;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
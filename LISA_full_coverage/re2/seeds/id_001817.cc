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
//<ID> 1817
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "cat";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "the cat sat on the cat";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char work_buf[128];
    char rewrite_buf[64];
    memset(work_buf, 0, sizeof(work_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    cre2_options_t * opt = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int replace_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    enc = cre2_opt_encoding(opt);
    memcpy(work_buf, source_text, (size_t)source_text_len);
    const char replacement[] = "dog";
    const int replacement_len = (int)(sizeof(replacement) - 1);
    memcpy(rewrite_buf, replacement, (size_t)replacement_len);

    // step 3: Configure
    text_and_target.data = work_buf;
    text_and_target.length = source_text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = replacement_len;

    // step 4: Operate
    replace_res = cre2_replace(pattern, &text_and_target, &rewrite);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)enc + (long)replace_res + (long)text_and_target.length + (long)rewrite.length + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
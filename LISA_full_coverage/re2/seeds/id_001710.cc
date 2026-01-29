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
//<ID> 1710
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(hello)(?: world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "hello world!";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t pmatch[4];
    int match_rc = 0;
    int prog_size = 0;
    int set_compile_rc = 0;
    size_t set_match_count = 0;
    int set_matches[8];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);

    // step 3: Configure
    prog_size = cre2_program_size(re);

    // step 4: Operate
    pmatch[0].data = NULL; pmatch[0].length = 0;
    pmatch[1].data = NULL; pmatch[1].length = 0;
    pmatch[2].data = NULL; pmatch[2].length = 0;
    pmatch[3].data = NULL; pmatch[3].length = 0;
    match_rc = cre2_match(re, text_buf, source_text_len, 0, source_text_len, CRE2_UNANCHORED, pmatch, 4);
    set_compile_rc = cre2_set_compile(set);
    memset(set_matches, 0, sizeof(set_matches));
    set_match_count = cre2_set_match(set, text_buf, (size_t)source_text_len, set_matches, sizeof(set_matches) / sizeof(set_matches[0]));

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)match_rc + (long)prog_size + (long)set_compile_rc + (long)set_match_count + (long)pmatch[0].length + (long)(pmatch[0].data != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
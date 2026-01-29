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
//<ID> 1719
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char extra_pattern[] = "baz";
    const int extra_pattern_len = (int)(sizeof(extra_pattern) - 1);
    const char source_text[] = "foobarbaz and foobar";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set_ptr = NULL;
    cre2_string_t pmatch[4];
    int match_result = 0;
    int prog_size = 0;
    int set_compile_res = 0;
    int set_match_indices[8];
    size_t set_matches = 0;
    char set_errbuf[64];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    set_ptr = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    memset(set_errbuf, 0, sizeof(set_errbuf));
    cre2_set_add_simple(set_ptr, "foo");
    cre2_set_add(set_ptr, extra_pattern, (size_t)extra_pattern_len, set_errbuf, sizeof(set_errbuf));
    pmatch[0].data = NULL; pmatch[0].length = 0;
    pmatch[1].data = NULL; pmatch[1].length = 0;
    pmatch[2].data = NULL; pmatch[2].length = 0;
    pmatch[3].data = NULL; pmatch[3].length = 0;
    memset(set_match_indices, 0, sizeof(set_match_indices));

    // step 4: Core operations
    set_compile_res = cre2_set_compile(set_ptr);
    prog_size = cre2_program_size(re);
    match_result = cre2_match(re, source_text, source_text_len, 0, source_text_len, CRE2_UNANCHORED, pmatch, 4);
    set_matches = cre2_set_match(set_ptr, source_text, (size_t)source_text_len, set_match_indices, sizeof(set_match_indices) / sizeof(set_match_indices[0]));

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)match_result + (long)prog_size + (long)set_compile_res + (long)set_matches + (long)pmatch[0].length + (long)pmatch[1].length + (long)(set_errbuf[0]);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set_ptr);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
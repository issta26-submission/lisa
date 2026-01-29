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
//<ID> 831
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(Hello) (World)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Hello World! Hello Universe!";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int case_flag = 1;
    char errbuf[128];
    memset(errbuf, 0, sizeof(errbuf));
    int add_res = 0;
    int compile_res = 0;
    int match_indices[4];
    memset(match_indices, 0, sizeof(match_indices));
    size_t nmatches = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Operate
    add_res = cre2_set_add(set, pattern, (size_t)pattern_len, errbuf, sizeof(errbuf));
    compile_res = cre2_set_compile(set);
    nmatches = cre2_set_match(set, text_buf, text_len, match_indices, (size_t)4);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate
    computed_check = (long)nmatches + (long)add_res + (long)compile_res + (long)enc + (long)case_flag + (long)match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
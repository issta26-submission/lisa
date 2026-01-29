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
//<ID> 2137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<word>[A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    char set_errbuf[64];
    const char subject_buf[] = "Alice-123";
    cre2_string_t subject = { subject_buf, (int)(sizeof(subject_buf) - 1) };
    cre2_string_t matches[4];
    char match_storage0[32];
    char match_storage1[32];
    matches[0].data = match_storage0;
    matches[0].length = 0;
    matches[1].data = match_storage1;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;
    matches[3].data = NULL;
    matches[3].length = 0;
    char tat_buf[] = "Name-000 Name-111";
    cre2_string_t tat = { tat_buf, (int)(sizeof(tat_buf) - 1) };
    char rewrite_buf[] = "$1_$2";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    int full_match_ret = 0;
    int replace_ret = 0;
    int set_add_ret = 0;
    int num_named_index = 0;
    int error_code = 0;
    long checksum = 0;

    // step 2: Setup (create options and inspect)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    int64_t maxmem = cre2_opt_max_mem(opt);

    // step 3: Configure (compile regexp and prepare set)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    memset(set_errbuf, 0, sizeof(set_errbuf));
    set_add_ret = cre2_set_add_simple(set, pattern);
    error_code = cre2_error_code(re);

    // step 4: Operate (create named-groups iterator and perform full match)
    iter = cre2_named_groups_iter_new(re);
    full_match_ret = cre2_full_match(pattern, &subject, matches, 3);

    // step 5: Operate (perform a replace using compiled regexp) and Validate (combine values)
    replace_ret = cre2_replace_re(re, &tat, &rewrite);
    num_named_index = cre2_find_named_capturing_groups ? 0 : 0;
    checksum = (long)pattern_len + (long)full_match_ret + (long)replace_ret + (long)set_add_ret + (long)error_code + (long)maxmem + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL) + (long)(iter != NULL);
    (void)checksum;
    (void)num_named_index;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
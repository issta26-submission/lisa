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
//<ID> 2131
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
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int64_t maxmem = 0;
    int add_simple_ret = 0;
    const char source_buf[] = "Bob-42";
    cre2_string_t text = { source_buf, (int)(sizeof(source_buf) - 1) };
    char whole_buf[64];
    char cap1buf[32];
    char cap2buf[32];
    cre2_string_t caps[3];
    char rewrite_buf[] = "$1:$2";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    char tat_buf[] = "Alice-123 Bob-456";
    cre2_string_t tat = { tat_buf, (int)(sizeof(tat_buf) - 1) };
    int full_match_ret = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)(1024 * 1024));
    maxmem = cre2_opt_max_mem(opt);

    // step 3: Configure (compile regexp and prepare set and iterator)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_simple_ret = cre2_set_add_simple(set, pattern);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (prepare capture buffers, run full match and perform replace)
    memset(whole_buf, 0, sizeof(whole_buf));
    memset(cap1buf, 0, sizeof(cap1buf));
    memset(cap2buf, 0, sizeof(cap2buf));
    caps[0].data = whole_buf;
    caps[0].length = 0;
    caps[1].data = cap1buf;
    caps[1].length = 0;
    caps[2].data = cap2buf;
    caps[2].length = 0;
    full_match_ret = cre2_full_match(pattern, &text, caps, 3);
    replace_ret = cre2_replace_re(re, &tat, &rewrite);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)full_match_ret + (long)replace_ret + (long)add_simple_ret + (long)maxmem + (long)(re != NULL) + (long)(set != NULL) + (long)(iter != NULL) + (long)tat.length + (long)text.length + (long)caps[1].length + (long)caps[2].length;
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
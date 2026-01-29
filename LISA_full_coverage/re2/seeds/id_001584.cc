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
//<ID> 1584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char rewrite_literal[] = "$1-$2";
    const int rewrite_len = 5; /* "$1-$2" length */
    char errmsg_buf[128];
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int check_res = 0;
    int ver_rev = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    rewrite.data = rewrite_literal;
    rewrite.length = rewrite_len;
    errmsg.data = errmsg_buf;
    errmsg.length = 0;
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate
    ver_rev = cre2_version_interface_revision();
    checksum = (long)pattern_len + (long)enc + (long)(re != NULL) + (long)check_res + (long)ver_rev + (long)rewrite.length + (long)errmsg.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
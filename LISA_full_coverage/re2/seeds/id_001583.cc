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
//<ID> 1583
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
    int ver_rev = 0;
    const char rewrite_txt[] = "$2-$1";
    cre2_string_t rewrite;
    char errmsg_buf[128];
    cre2_string_t errmsg;
    int check_result = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    ver_rev = cre2_version_interface_revision();

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    rewrite.data = rewrite_txt;
    rewrite.length = (int)(sizeof(rewrite_txt) - 1);
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    errmsg.data = errmsg_buf;
    errmsg.length = (int)sizeof(errmsg_buf);
    check_result = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 4: Validate
    checksum = (long)pattern_len + (long)enc + (long)ver_rev + (long)(re != NULL) + (long)check_result + (long)rewrite.length + (long)errmsg.length;
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}
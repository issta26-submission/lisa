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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for the set
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_case_sensitive(opt, 0);

    // Create a set, add simple patterns and compile it
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "baz");
    cre2_set_compile(set);

    // Prepare text and call cre2_set_match
    const char *text = "this has foo and baz";
    size_t text_len = (size_t)strlen(text);
    int matches[8];
    for (size_t i = 0; i < sizeof(matches)/sizeof(matches[0]); ++i) matches[i] = -1;
    size_t nmatched = cre2_set_match(set, text, text_len, matches, (size_t)(sizeof(matches)/sizeof(matches[0])));

    // Prepare strings for cre2_replace
    std::string replace_subject = "replace foo with qux";
    cre2_string_t text_and_target = { replace_subject.c_str(), (int)replace_subject.size() };
    std::string rewrite_s = "qux";
    cre2_string_t rewrite = { rewrite_s.c_str(), (int)rewrite_s.size() };
    // Use cre2_replace to attempt a single replacement
    int replace_rc = cre2_replace("foo", &text_and_target, &rewrite);
    (void)replace_rc; // ignore return code in this test sequence

    // Prepare buffers for consume / find_and_consume calls
    char consume_buf[64];
    strncpy(consume_buf, "123abc456", sizeof(consume_buf));
    cre2_string_t consume_text = { consume_buf, (int)strlen(consume_buf) };

    char match_buf[64];
    match_buf[0] = '\0';
    cre2_string_t match_out = { match_buf, 0 };

    // Use cre2_consume to consume a numeric prefix (pattern uses \d+, escape for C string)
    int cons_rc = cre2_consume("\\d+", &consume_text, &match_out, 1);
    (void)cons_rc;

    // Reset buffers and use cre2_find_and_consume to find and consume a literal
    strncpy(consume_buf, "abcXYZdef", sizeof(consume_buf));
    consume_text.data = consume_buf;
    consume_text.length = (int)strlen(consume_buf);
    match_buf[0] = '\0';
    match_out.data = match_buf;
    match_out.length = 0;

    int find_consume_rc = cre2_find_and_consume("XYZ", &consume_text, &match_out, 1);
    (void)find_consume_rc;

    // Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // If we reached here, consider it success for this test harness
    return 66;
}
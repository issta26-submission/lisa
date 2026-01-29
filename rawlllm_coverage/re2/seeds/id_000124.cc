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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set a couple of flags.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // Create a pattern set, add simple patterns, compile it, and match against some text.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) { cre2_opt_delete(opt); return 2; }
    cre2_set_add_simple(set, "hello");
    cre2_set_add_simple(set, "world");
    cre2_set_compile(set);

    const char *input = "say hello world!";
    int match_indices[8] = { -1 };
    size_t set_matches = cre2_set_match(set, input, strlen(input), match_indices, 8);
    (void)set_matches; // result used only to exercise the API

    // Prepare a mutable buffer for replacements (avoid passing string literals if the API expects writable memory).
    char buf[128];
    const char *initial = "say hello world!";
    size_t buflen = strlen(initial);
    if (buflen >= sizeof(buf)) buflen = sizeof(buf) - 1;
    memcpy(buf, initial, buflen);
    buf[buflen] = '\0';

    cre2_string_t text_and_target;
    text_and_target.data = buf;
    text_and_target.length = (int)buflen;

    // Compile a regex to replace "world".
    const char *pattern = "world";
    cre2_regexp_t *rex = cre2_new(pattern, (int)strlen(pattern), opt);

    // Prepare rewrite string "earth".
    const char rewrite_data[] = "earth";
    cre2_string_t rewrite;
    rewrite.data = rewrite_data;
    rewrite.length = (int)strlen(rewrite_data);

    // Do a single replace using compiled regex and then a global replace.
    if (rex) {
        int r1 = cre2_replace_re(rex, &text_and_target, &rewrite);
        (void)r1;
        int r2 = cre2_global_replace_re(rex, &text_and_target, &rewrite);
        (void)r2;
    }

    // Use cre2_find_and_consume (pattern-string variant) to find and consume from a subject.
    char subj_buf[64];
    const char *subj_init = "hello world!";
    size_t subj_len = strlen(subj_init);
    if (subj_len >= sizeof(subj_buf)) subj_len = sizeof(subj_buf) - 1;
    memcpy(subj_buf, subj_init, subj_len);
    subj_buf[subj_len] = '\0';
    cre2_string_t subject;
    subject.data = subj_buf;
    subject.length = (int)subj_len;

    // Prepare capture array (2 slots: full match + one group).
    cre2_string_t caps[2];
    caps[0].data = NULL; caps[0].length = 0;
    caps[1].data = NULL; caps[1].length = 0;

    int found = cre2_find_and_consume("hello (world)", &subject, caps, 2);
    (void)found; // only exercising API

    // Cleanup
    if (rex) cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}
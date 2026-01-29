#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *s_alpha = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s_alpha);
    cJSON *s_beta = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s_beta);
    cJSON *b_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, b_true);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("device");
    cJSON_AddItemToObject(meta, "name", name);
    char keybuf[16];
    memset(keybuf, 0, sizeof(keybuf));
    keybuf[0] = 's'; keybuf[1] = 't'; keybuf[2] = 'a'; keybuf[3] = 't'; keybuf[4] = 'u'; keybuf[5] = 's'; keybuf[6] = '\0';
    cJSON_AddItemToObject(meta, keybuf, cJSON_CreateString("ok"));

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("BETA_REPLACED");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, replacement);
    cJSON_bool true_flag = cJSON_IsTrue(b_true);
    const char *repl_label = cJSON_GetStringValue(replacement);
    cJSON *flag_num = cJSON_CreateNumber((double)true_flag);
    cJSON_AddItemToObject(meta, "flag", flag_num);
    cJSON *repl_copy = cJSON_CreateString(repl_label ? repl_label : "");
    cJSON_AddItemToObject(meta, "repl_label", repl_copy);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
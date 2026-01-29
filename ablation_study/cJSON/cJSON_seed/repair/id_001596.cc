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
//<ID> 1596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(child, "pi", pi_item);
    cJSON *seven_item = cJSON_AddNumberToObject(child, "seven", 7.0);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemReferenceToObject(root, "greeting_ref", greeting);

    // step 2: Configure
    cJSON *got_greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting_ref");
    cJSON *got_child = cJSON_GetObjectItemCaseSensitive(root, "child");
    double pi_val = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(got_child, "pi"));
    cJSON *double_pi = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "double_pi", double_pi);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (out_len > 0) ? out[0] : ' ';
    scratch[1] = (got_greeting && got_greeting->valuestring) ? got_greeting->valuestring[0] : ' ';
    scratch[2] = (char)('0' + (int)cJSON_IsNumber(cJSON_GetObjectItemCaseSensitive(child, "seven")));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
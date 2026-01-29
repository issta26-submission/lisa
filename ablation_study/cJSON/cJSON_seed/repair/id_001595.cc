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
//<ID> 1595
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
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(child, "greeting", greeting);
    cJSON *primary_num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(child, "primary", primary_num);

    // step 2: Configure
    cJSON_AddNumberToObject(child, "value", 3.14159);
    cJSON_AddItemReferenceToObject(root, "primary_alias", primary_num);

    // step 3: Operate and Validate
    cJSON *child_ref = cJSON_GetObjectItemCaseSensitive(root, "child");
    cJSON *value_ref = cJSON_GetObjectItemCaseSensitive(child_ref, "value");
    double value_val = cJSON_GetNumberValue(value_ref);
    cJSON *double_val = cJSON_CreateNumber(value_val * 2.0);
    cJSON_AddItemToObject(root, "double_value", double_val);
    cJSON *alias_ref = cJSON_GetObjectItemCaseSensitive(root, "primary_alias");
    double alias_val = cJSON_GetNumberValue(alias_ref);
    cJSON *status = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "status", status);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)value_val);
    scratch[1] = (char)('0' + (int)alias_val);
    scratch[2] = (char)('0' + (int)(alias_val != 0.0));
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
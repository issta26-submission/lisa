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
//<ID> 478
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddStringToObject(root, "name", "test-case");
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToArray(arr, raw);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, false_item);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "active");
    char info_buf[128];
    memset(info_buf, 0, sizeof(info_buf));

    // step 3: Operate & Validate
    cJSON *arr_true = cJSON_GetArrayItem(arr, 1);
    cJSON *arr_false = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_t = cJSON_IsTrue(arr_true);
    cJSON_bool is_f = cJSON_IsFalse(arr_false);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    sprintf(info_buf, "ver=%s|name=%s|true=%d|false=%d", version ? version : "unknown", name_val ? name_val : "", (int)is_t, (int)is_f);
    (void)info_buf;
    cJSON_DeleteItemFromArray(arr, 0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool same = cJSON_Compare(root, parsed, 1);
    (void)same;
    cJSON_free(snapshot);
    cJSON_Delete(parsed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
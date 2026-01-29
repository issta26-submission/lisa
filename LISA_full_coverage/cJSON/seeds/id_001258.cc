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
//<ID> 1258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON_AddNumberToObject(config, "threshold", 3.14);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "mode", "fast");
    cJSON_AddItemToObject(config, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));

    // step 2: Configure
    cJSON *ref_obj = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref_obj);
    cJSON *extra = cJSON_CreateString("extra");
    cJSON_AddItemToObject(root, "note", extra);

    // step 3: Operate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_child = cJSON_GetObjectItem(got_config, "child");
    cJSON *detached_child = cJSON_DetachItemViaPointer(got_config, got_child);
    cJSON_bool is_detached_invalid = cJSON_IsInvalid(detached_child);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    int buf_len = (int)printed_len + 32;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    memcpy(buf, printed, printed_len + 1);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON *enabled_item = cJSON_GetObjectItem(got_config, "enabled");
    double threshold_val = cJSON_GetNumberValue(cJSON_GetObjectItem(got_config, "threshold"));
    (void)threshold_val;
    (void)is_detached_invalid;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(detached_child);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
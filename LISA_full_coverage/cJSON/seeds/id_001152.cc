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
//<ID> 1152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "items", arr);
    cJSON *s = cJSON_CreateString("original");
    cJSON_AddItemToArray(arr, s);
    cJSON *n = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, n);
    cJSON *sref = cJSON_CreateStringReference("ref");
    cJSON_AddItemToArray(arr, sref);
    char *updated_ptr = cJSON_SetValuestring(s, "updated");
    cJSON_AddStringToObject(root, "updated_value", updated_ptr);

    // step 2: Operate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "root_is_object", root_is_obj);
    cJSON_bool sref_invalid = cJSON_IsInvalid(sref);
    cJSON_AddBoolToObject(root, "sref_invalid", sref_invalid);
    cJSON *detached = cJSON_DetachItemFromObject(root, "child");
    cJSON_AddItemToObject(root, "rechild", detached);
    cJSON_bool detached_is_obj = cJSON_IsObject(detached);
    cJSON_AddBoolToObject(root, "detached_is_object", detached_is_obj);

    // step 3: Validate
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
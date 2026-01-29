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
//<ID> 1027
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"tester\",\"nums\":[10,20,30],\"nested\":{\"msg\":\"hello\",\"val\":5}}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *nums_item = cJSON_GetObjectItem(root, "nums");
    cJSON *nested_item = cJSON_GetObjectItem(root, "nested");
    cJSON *nested_msg = cJSON_GetObjectItem(nested_item, "msg");
    const char *msg_str = cJSON_GetStringValue(nested_msg);
    cJSON *duplicated_nested = cJSON_Duplicate(nested_item, 1);
    cJSON *num0 = cJSON_GetArrayItem(nums_item, 0);
    cJSON *num1 = cJSON_GetArrayItem(nums_item, 1);
    cJSON *num2 = cJSON_GetArrayItem(nums_item, 2);
    int int_buf[3];
    int_buf[0] = (int)cJSON_GetNumberValue(num0);
    int_buf[1] = (int)cJSON_GetNumberValue(num1);
    int_buf[2] = (int)cJSON_GetNumberValue(num2);
    cJSON *int_array = cJSON_CreateIntArray(int_buf, 3);

    // step 3: Operate
    cJSON *out = cJSON_CreateObject();
    cJSON *msg_obj = cJSON_CreateString(msg_str);
    cJSON_AddItemToObject(out, "copied_msg", msg_obj);
    cJSON_AddItemToObject(out, "dup_nested", duplicated_nested);
    cJSON_AddItemToObject(out, "copied_nums", int_array);
    char *out_str = cJSON_PrintUnformatted(out);
    char *prebuf = (char *)cJSON_malloc(256);
    memset(prebuf, 0, 256);
    cJSON_PrintPreallocated(out, prebuf, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_free(out_str);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1026
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"example\",\"numbers\":[1,2,3],\"nested\":{\"msg\":\"hello\"}}";
    size_t json_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, json_len);

    // step 2: Configure
    cJSON *nested = cJSON_GetObjectItem(root, "nested");
    cJSON *msg_item = cJSON_GetObjectItem(nested, "msg");
    const char *msg_str = cJSON_GetStringValue(msg_item);
    cJSON *dup_nested = cJSON_Duplicate(nested, 1);
    int nums[] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);
    cJSON *out = cJSON_CreateObject();

    // step 3: Operate
    cJSON *copied_msg = cJSON_CreateString(msg_str);
    cJSON_AddItemToObject(out, "ints", int_array);
    cJSON_AddItemToObject(out, "copied_msg", copied_msg);
    cJSON_AddItemToObject(out, "dup_nested", dup_nested);
    char *out_unformatted = cJSON_PrintUnformatted(out);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(out, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}
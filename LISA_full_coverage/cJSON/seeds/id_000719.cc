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
//<ID> 719
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"cjson\",\"values\":[10,20,30]}";
    size_t json_len = (size_t) ( (const char *) (json_text + 0) - json_text ) + strlen(json_text); // compute length deterministically
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    int nums[] = {7, 8, 9};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "int_array", int_arr);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(int_arr);
    cJSON *size_marker = cJSON_CreateNumber((double)size_before);
    cJSON_AddItemToObject(root, "size_before", size_marker);

    // step 3: Operate
    cJSON *dup_arr = cJSON_Duplicate(int_arr, 1);
    cJSON_AddItemToObject(root, "duplicated_array", dup_arr);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);

    // step 4: Validate & Cleanup
    int final_count = cJSON_GetArraySize(dup_arr);
    cJSON_AddItemToObject(root, "final_count", cJSON_CreateNumber((double)final_count));
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
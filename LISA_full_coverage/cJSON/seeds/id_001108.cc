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
//<ID> 1108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"initial\":true}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    double nums[] = {1.1, 2.2, 3.3};
    cJSON *double_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "doubles", double_array);
    cJSON *raw_item = cJSON_CreateRaw("some_raw_text");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON *name_item = cJSON_CreateString("old_name");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate
    cJSON *retrieved_array = cJSON_GetObjectItem(root, "doubles");
    cJSON *first_elem = cJSON_GetArrayItem(retrieved_array, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first_elem);
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    double checks = (double)raw_check + (double)first_is_number;
    cJSON_AddNumberToObject(root, "checks", checks);
    cJSON_SetValuestring(name_item, "updated_name");

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
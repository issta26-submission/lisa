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
//<ID> 956
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\": {\"numbers\": [1, 2]}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *nums = cJSON_GetObjectItem(root_obj, "numbers");
    cJSON *new_num = cJSON_CreateNumber(3.5);
    cJSON_InsertItemInArray(nums, 2, new_num);
    cJSON *meta = cJSON_AddObjectToObject(root_obj, "meta");
    cJSON *label_ref = cJSON_CreateStringReference("reference_label");
    cJSON_AddItemToObject(meta, "label", label_ref);
    cJSON *arr_ref = cJSON_CreateArrayReference(nums);
    cJSON_AddItemToObject(root_obj, "numbers_ref", arr_ref);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
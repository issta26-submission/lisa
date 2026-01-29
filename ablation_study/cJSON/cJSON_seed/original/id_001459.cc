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
//<ID> 1459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"data\":{\"nums\":[1,2,3]},\"name\":\"root\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *nums = cJSON_GetObjectItem(data, "nums");

    // step 2: Configure
    cJSON *new_number = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(nums, new_number);
    cJSON *created_array = cJSON_CreateArray();
    cJSON_AddItemToArray(created_array, cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(created_array, cJSON_CreateNumber(8.0));
    cJSON_AddItemToObject(root, "extra", created_array);
    cJSON *array_ref = cJSON_CreateArrayReference(created_array);
    cJSON_AddItemToObject(root, "extra_ref", array_ref);

    // step 3: Operate & Validate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *first_num = cJSON_GetArrayItem(nums, 0);
    double first_val = cJSON_GetNumberValue(first_num);
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "extra_ref");
    (void)first_val;
    (void)has_ref;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
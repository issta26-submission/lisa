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
//<ID> 627
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    int nums[3] = { 10, 20, 30 };
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "empty", null_item);

    // step 2: Configure
    int size = cJSON_GetArraySize(int_array);
    cJSON_AddNumberToObject(root, "count", (double)size);
    const int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_ints = cJSON_GetObjectItem(parsed, "ints");
    cJSON *first_item = cJSON_GetArrayItem(parsed_ints, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *parsed_empty = cJSON_GetObjectItem(parsed, "empty");
    cJSON_bool empty_is_null = cJSON_IsNull(parsed_empty);
    (void)version;
    (void)first_value;
    (void)empty_is_null;

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "empty");
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
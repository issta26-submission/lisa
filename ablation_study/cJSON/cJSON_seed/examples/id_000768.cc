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
//<ID> 768
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddItemToObject(root, "vals", array);
    cJSON_AddItemToObject(root, "nullable", null_item);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);

    // step 4: Validate & Cleanup
    cJSON *parsed_nullable = cJSON_GetObjectItem(parsed, "nullable");
    cJSON_bool is_null = cJSON_IsNull(parsed_nullable);
    cJSON *parsed_pi = cJSON_GetObjectItem(parsed, "pi");
    double pi_val = cJSON_GetNumberValue(parsed_pi);
    cJSON *parsed_vals = cJSON_GetObjectItem(parsed, "vals");
    int arr_size = cJSON_GetArraySize(parsed_vals);
    cJSON *new_item = cJSON_CreateNumber(pi_val + (double)arr_size);
    cJSON_AddItemToObject(parsed, "pi_plus_count", new_item);
    cJSON *retrieved = cJSON_GetObjectItem(parsed, "pi_plus_count");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    (void)is_null;
    (void)retrieved_val;

    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
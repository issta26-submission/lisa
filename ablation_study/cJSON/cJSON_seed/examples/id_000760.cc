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
//<ID> 760
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *nullable = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "nullable", nullable);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_nullable = cJSON_GetObjectItem(parsed, "nullable");
    cJSON_bool is_null = cJSON_IsNull(parsed_nullable);
    cJSON *parsed_pi = cJSON_GetObjectItem(parsed, "pi");
    double pi_value = cJSON_GetNumberValue(parsed_pi);
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "arr");
    int arr_size = cJSON_GetArraySize(parsed_arr);
    (void)is_null;
    (void)pi_value;
    (void)arr_size;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
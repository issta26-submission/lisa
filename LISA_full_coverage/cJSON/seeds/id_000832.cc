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
//<ID> 832
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"alpha\": [10,20], \"active\": true}";
    size_t len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, len);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "original", parsed);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extras", arr);
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddRawToObject(root, "inlined", "{\"raw_inner\":42}");
    cJSON_AddBoolToObject(root, "flagged", 0);

    // step 3: Operate
    char *compact = cJSON_PrintUnformatted(root);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    cJSON_bool is_array = cJSON_IsArray(arr);
    (void)first_val;
    (void)is_array;
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
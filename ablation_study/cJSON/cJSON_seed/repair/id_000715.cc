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
//<ID> 715
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"example\",\"values\":[10,20,30]}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    int native_ints[3] = {7, 8, 9};
    cJSON *int_array = cJSON_CreateIntArray(native_ints, 3);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "int_array", int_array);

    // step 3: Operate and Validate
    cJSON *detached_item = cJSON_DetachItemFromArray(int_array, 1);
    cJSON_AddItemToObjectCS(root, "detached_item", detached_item);
    int remaining = cJSON_GetArraySize(int_array);
    cJSON_AddNumberToObject(root, "int_array_remaining", (double)remaining);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *first_val = cJSON_GetArrayItem(values, 0);
    double first_number = cJSON_GetNumberValue(first_val);
    cJSON_AddNumberToObject(root, "first_value_in_parsed", first_number);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
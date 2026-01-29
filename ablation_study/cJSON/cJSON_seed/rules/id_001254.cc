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
//<ID> 1254
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    double nums[] = { 1.0, 2.5, -3.75 };
    cJSON *root = cJSON_CreateObject();
    cJSON *num_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *retrieved_item = NULL;
    char buf[64];
    char *printed = NULL;
    double second_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", num_array);
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddRawToObject(root, "meta", "{\"embedded\":true,\"count\":3}");
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 3: Operate / Validate
    retrieved_item = cJSON_GetArrayItem(num_array, 1);
    second_value = cJSON_GetNumberValue(retrieved_item);
    snprintf(buf, sizeof(buf), "value: %.6g", second_value);
    cJSON_AddStringToObject(root, "second_value", buf);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"values\":[1.0,2.0],\"meta\":{\"name\":\"example\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    double extra_nums[2] = {3.1415, 2.71828};
    cJSON *extra_array = cJSON_CreateDoubleArray(extra_nums, 2);
    cJSON_AddItemToObject(root, "extra", extra_array);
    cJSON_AddFalseToObject(root, "enabled");
    cJSON *push_array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(push_array, n1);
    cJSON_AddItemToObject(root, "pushes", push_array);

    // step 3: Operate and Validate
    cJSON *flag = cJSON_GetObjectItem(root, "enabled");
    double flag_val = (double)cJSON_IsTrue(flag);
    cJSON_AddNumberToObject(root, "enabled_num", flag_val);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    int values_count = cJSON_GetArraySize(values);
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
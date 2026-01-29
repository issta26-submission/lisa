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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"example\",\"values\":[1,2]}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    double numbers[3] = {3.14, 2.718, 1.618};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "double_array", double_array);
    cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate and Validate
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);
    cJSON_AddNumberToObject(root, "enabled_numeric", (double)enabled_flag);
    cJSON *container = cJSON_CreateArray();
    cJSON *dup_array = cJSON_Duplicate(double_array, 1);
    cJSON_AddItemToArray(container, dup_array);
    cJSON_AddItemToObject(root, "container", container);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
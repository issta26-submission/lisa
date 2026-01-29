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
//<ID> 501
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *count = cJSON_AddNumberToObject(root, "count", 4.0);

    // step 2: Configure
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(nested, "values", array);
    cJSON *v0 = cJSON_CreateNumber(10.0);
    cJSON *v1 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(array, v0);
    cJSON_AddItemToArray(array, v1);

    // step 3: Operate
    double cnt = cJSON_GetNumberValue(count);
    cJSON *scaled = cJSON_CreateNumber(cnt * 1.5);
    cJSON_AddItemToArray(array, scaled);
    cJSON *newname = cJSON_CreateString("updated_label");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, name, newname);
    (void)replaced;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    int size = cJSON_GetArraySize(array);
    cJSON *last = cJSON_GetArrayItem(array, size - 1);
    double last_val = cJSON_GetNumberValue(last);
    (void)last_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
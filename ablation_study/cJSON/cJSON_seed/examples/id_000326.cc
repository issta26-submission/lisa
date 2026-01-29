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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(child, "value", num_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "to_remove", null_item);
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    double new_val = cJSON_SetNumberHelper(num_item, 42.0);
    cJSON_AddItemToObject(root, "value_copy", cJSON_CreateNumber(new_val));

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "to_remove");
    char *printed = cJSON_Print(root);
    cJSON_AddItemToObject(root, "printed", cJSON_CreateString(printed));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
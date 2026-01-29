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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "root_obj", obj);
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(obj, "value", num);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(obj, "nullable", nul);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *s0 = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, s0);

    // step 2: Configure
    double new_val = cJSON_SetNumberHelper(num, 3.1415);
    cJSON *copy_num = cJSON_CreateNumber(new_val);
    cJSON_AddItemToObject(root, "copy_value", copy_num);
    cJSON_AddItemToObject(obj, "temp", cJSON_CreateString("to_be_removed"));

    // step 3: Operate & Validate
    char *printed_before = cJSON_Print(root);
    cJSON_DeleteItemFromObjectCaseSensitive(obj, "temp");
    char *printed_after = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
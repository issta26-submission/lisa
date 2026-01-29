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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.0);
    const char *text = "hello";
    cJSON *strref = cJSON_CreateStringReference(text);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, strref);
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddTrueToObject(nested, "flag");
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate
    cJSON *nested_ref = cJSON_CreateObjectReference(nested);
    cJSON_AddItemToObject(root, "nested_ref", nested_ref);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(nested, "moved", detached);
    char *retrieved = cJSON_GetStringValue(strref);
    cJSON *strref2 = cJSON_CreateStringReference(retrieved);
    cJSON_AddItemToObject(nested, "str_copy", strref2);
    cJSON_DeleteItemFromObject(root, "nested_ref");

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
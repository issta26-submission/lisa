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
//<ID> 392
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
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, s1);
    cJSON_InsertItemInArray(arr, 1, s2);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate
    cJSON *det = cJSON_DetachItemFromArray(arr, 0);
    cJSON_SetValuestring(det, "modified");
    cJSON_InsertItemInArray(arr, 0, det);

    // step 4: Validate and Cleanup
    const char *first_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    cJSON_AddStringToObject(root, "detached_value", first_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
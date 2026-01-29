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
//<ID> 8
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
    cJSON *b1 = cJSON_CreateTrue();
    cJSON *s1 = cJSON_CreateString("initial");

    // step 2: Configure
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, b1);
    cJSON_SetValuestring(s1, "modified");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddBoolToObject(root, "flag", 0);

    // step 3: Operate & Validate
    cJSON_bool was_bool = cJSON_IsBool(b1);
    s1->valueint = (int)was_bool;
    cJSON_DeleteItemFromArray(arr, 0);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
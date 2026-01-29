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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *strItem = (cJSON *)0;
    cJSON *numItem = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *detached_text = (char *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double numeric_value = 0.0;
    double numeric_confirm = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    strItem = cJSON_CreateString("to_be_moved");
    cJSON_AddItemToArray(arr, strItem);
    numItem = cJSON_AddNumberToObject(root, "count", 7.25);

    // step 3: Operate
    is_array = cJSON_IsArray(arr);
    numeric_value = cJSON_GetNumberValue(numItem) * (double)is_array;
    detached = cJSON_DetachItemViaPointer(arr, strItem);
    cJSON_AddItemToObject(root, "moved_string", detached);

    // step 4: Validate
    detached_text = cJSON_GetStringValue(detached);
    numeric_confirm = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
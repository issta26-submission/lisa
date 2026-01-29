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
//<ID> 1439
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *bool_item = NULL;
    cJSON *str_item = NULL;
    cJSON *detached = NULL;
    char *strval = NULL;
    cJSON_bool raw_check = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    bool_item = cJSON_CreateFalse();
    str_item = cJSON_CreateString("detached_string");
    cJSON_AddItemToArray(arr, bool_item);
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate / Validate
    raw_check = cJSON_IsRaw(str_item);
    detached = cJSON_DetachItemViaPointer(arr, str_item);
    strval = cJSON_GetStringValue(detached);
    (void)raw_check;
    (void)strval;

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
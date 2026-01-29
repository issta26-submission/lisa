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
//<ID> 1437
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
    cJSON *str_item = NULL;
    cJSON *item_false = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    char *strval = NULL;
    cJSON_bool is_raw = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("detached_value");
    cJSON_AddItemToObject(root, "to_detach", str_item);
    arr = cJSON_CreateArray();
    item_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToObject(root, "array", arr);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemViaPointer(root, str_item);
    is_raw = cJSON_IsRaw(detached);
    strval = cJSON_GetStringValue(detached);
    cJSON_AddStringToObject(root, "copy", strval);
    cJSON_AddItemToArray(arr, detached);
    printed = cJSON_PrintBuffered(root, 128, is_raw);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1434
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
    cJSON *item_false = NULL;
    cJSON *str = NULL;
    cJSON *str_copy = NULL;
    cJSON *raw = NULL;
    cJSON *detached_raw = NULL;
    char *strval = NULL;
    char *printed = NULL;
    cJSON_bool is_raw = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToObject(root, "array", arr);
    str = cJSON_CreateString("greeting");
    cJSON_AddItemToObject(root, "hello", str);
    raw = cJSON_CreateRaw("{\"x\":1}");
    cJSON_AddItemToObject(root, "rawval", raw);

    // step 3: Operate / Validate
    is_raw = cJSON_IsRaw(raw);
    strval = cJSON_GetStringValue(str);
    str_copy = cJSON_CreateString(strval);
    cJSON_AddItemToObject(root, "hello_copy", str_copy);
    printed = cJSON_PrintUnformatted(root);
    detached_raw = cJSON_DetachItemViaPointer(root, raw);
    cJSON_AddItemToArray(arr, detached_raw);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
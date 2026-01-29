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
//<ID> 1435
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
    cJSON *false_item = NULL;
    cJSON *str_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    char prebuf[128];
    char *str_val = NULL;
    cJSON_bool raw_check = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    str_item = cJSON_CreateString("hello world");
    raw_item = cJSON_CreateRaw("unparsed,raw,content");
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate / Validate
    str_val = cJSON_GetStringValue(str_item);
    raw_check = cJSON_IsRaw(raw_item);
    detached = cJSON_DetachItemViaPointer(arr, false_item);
    printed = cJSON_Print(root);
    memset(prebuf, 0, sizeof(prebuf));
    (void)str_val;
    (void)raw_check;

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
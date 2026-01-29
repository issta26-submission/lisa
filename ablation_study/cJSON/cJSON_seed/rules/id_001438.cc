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
//<ID> 1438
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
    cJSON *str = NULL;
    cJSON *f = NULL;
    cJSON *raw = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    char prebuf[256];
    cJSON_bool pre_ok = 0;
    cJSON_bool israw = 0;
    const char *sval = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str = cJSON_CreateString("hello");
    f = cJSON_CreateFalse();
    raw = cJSON_CreateRaw("rawcontent");
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToArray(arr, f);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "raw", raw);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 256, 1);
    memset(prebuf, 0, sizeof(prebuf));
    pre_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    sval = cJSON_GetStringValue(str);
    israw = cJSON_IsRaw(raw);
    detached = cJSON_DetachItemViaPointer(arr, str);
    cJSON_AddItemToArray(arr, detached);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
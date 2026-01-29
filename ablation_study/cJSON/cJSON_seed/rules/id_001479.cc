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
//<ID> 1479
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
    cJSON *nested = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *arr_item = NULL;
    cJSON *detached = NULL;
    cJSON *pi = NULL;
    cJSON *dup_pi = NULL;
    char prebuf[128];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(nested, "pi", num);
    str = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate / Validate
    pi = cJSON_GetObjectItem(nested, "pi");
    dup_pi = cJSON_Duplicate(pi, 0);
    arr_item = cJSON_GetArrayItem(arr, 0);
    detached = cJSON_DetachItemViaPointer(arr, arr_item);
    (void)prebuf;
    (void)dup_pi;
    (void)detached;

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(dup_pi);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
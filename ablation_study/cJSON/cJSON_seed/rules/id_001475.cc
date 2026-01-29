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
//<ID> 1475
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *dup_item = NULL;
    cJSON *got_child = NULL;
    cJSON *array_item = NULL;
    cJSON *detached_item = NULL;
    char prebuf[128];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "items", arr);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    dup_item = cJSON_Duplicate(num_item, 1);
    cJSON_AddItemToObject(root, "dup_num", dup_item);

    // step 3: Operate / Validate
    got_child = cJSON_GetObjectItem(root, "child");
    array_item = cJSON_GetArrayItem(arr, 0);
    detached_item = cJSON_DetachItemViaPointer(arr, array_item);
    cJSON_AddItemToObject(root, "detached", detached_item);
    (void)got_child;
    (void)prebuf;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
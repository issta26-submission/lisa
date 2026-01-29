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
//<ID> 399
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
    cJSON *s3 = cJSON_CreateString("third");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s3);
    cJSON_InsertItemInArray(arr, 1, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", t);

    // step 2: Configure
    char *updated = cJSON_SetValuestring(s1, "updated_first");
    (void)updated;
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_InsertItemInArray(arr, 2, detached);

    // step 3: Operate
    cJSON *new_head = cJSON_CreateString("new_head");
    cJSON_InsertItemInArray(arr, 0, new_head);

    // step 4: Validate and Cleanup
    int count = cJSON_GetArraySize(arr);
    (void)count;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
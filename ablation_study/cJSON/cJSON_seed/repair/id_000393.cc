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
//<ID> 393
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON *t = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_InsertItemInArray(arr, 0, s2);
    cJSON_InsertItemInArray(arr, 1, s1);
    cJSON_InsertItemInArray(arr, 2, t);
    cJSON_AddItemToObject(root, "label", s3);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    char *prev = cJSON_SetValuestring(detached, "beta-modified");
    (void)prev;
    cJSON_InsertItemInArray(arr, 1, detached);

    // step 4: Validate and Cleanup
    const char *first = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    const char *second = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    (void)first;
    (void)second;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
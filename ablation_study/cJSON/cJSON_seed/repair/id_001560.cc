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
//<ID> 1560
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *first = cJSON_CreateString("first");
    cJSON *third = cJSON_CreateString("third");
    cJSON_AddItemToArray(arr, first);
    cJSON_AddItemToArray(arr, third);
    cJSON *detachable = cJSON_CreateString("detach_me");
    cJSON_AddItemToObject(root, "removable", detachable);

    // step 2: Configure
    cJSON *second = cJSON_CreateString("second");
    cJSON_InsertItemInArray(arr, 1, second);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *mid_item = cJSON_GetArrayItem(arr, 1);
    const char *mid_val = cJSON_GetStringValue(mid_item);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t plen = printed ? strlen(printed) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(plen + 16);
    memset(scratch, 0, plen + 16);
    scratch[0] = printed ? printed[0] : ' ';
    scratch[1] = (char)('0' + (size > 9 ? 9 : size));
    scratch[2] = mid_val ? mid_val[0] : ' ';
    cJSON *detached = cJSON_DetachItemFromObject(root, "removable");
    const char *detached_val = cJSON_GetStringValue(detached);
    scratch[3] = detached_val ? detached_val[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
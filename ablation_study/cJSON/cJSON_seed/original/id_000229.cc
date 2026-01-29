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
//<ID> 229
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
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemReferenceToArray(arr, str);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON_AddBoolToObject(root, "detached_was_false", cJSON_IsFalse(detached));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    const char *det_str = cJSON_GetStringValue(detached);
    int summary = arr_size + (int)first_val + (det_str ? (int)det_str[0] : 0) + (int)cJSON_IsFalse(first);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
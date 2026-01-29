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
//<ID> 1563
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_InsertItemInArray(arr, 1, s3);
    int size = cJSON_GetArraySize(arr);
    cJSON *picked = cJSON_GetArrayItem(arr, 1);
    char *picked_str = cJSON_GetStringValue(picked);
    cJSON *detached = cJSON_DetachItemFromObject(root, "items");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(detached, 64, 1);
    int buf_len = 128;
    char *scratch = (char *)cJSON_malloc((size_t)buf_len);
    memset(scratch, 0, (size_t)buf_len);
    scratch[0] = (char)('0' + (size % 10));
    scratch[1] = printed[0];
    scratch[2] = picked_str[0];

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
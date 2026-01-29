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
//<ID> 2010
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
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *sref = cJSON_CreateStringReference("beta_ref");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, sref);
    cJSON *s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s2);

    // step 3: Operate and Validate
    cJSON *target = cJSON_GetArrayItem(arr, 1);
    cJSON *replacement = cJSON_CreateString("beta_replaced");
    cJSON_ReplaceItemViaPointer(arr, target, replacement);
    char *printed = cJSON_Print(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = printed[1];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
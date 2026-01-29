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
//<ID> 2132
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

    // step 2: Configure
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, nul);
    cJSON *label = cJSON_AddStringToObject(root, "label", "init_label");

    // step 3: Operate and Validate
    char *ret1 = cJSON_SetValuestring(str1, "alpha_updated");
    char *ret2 = cJSON_SetValuestring(label, "final_label");
    cJSON_DeleteItemFromArray(arr, 2);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *v1 = cJSON_GetStringValue(str1);
    const char *v2 = cJSON_GetStringValue(label);
    buffer[0] = v1 ? v1[0] : '\0';
    buffer[1] = v2 ? v2[0] : '\0';
    buffer[2] = ret1 ? ret1[0] : '\0';
    buffer[3] = ret2 ? ret2[0] : '\0';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
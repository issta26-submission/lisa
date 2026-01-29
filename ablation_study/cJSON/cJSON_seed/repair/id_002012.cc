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
//<ID> 2012
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, s2);
    cJSON *sref = cJSON_CreateStringReference("gamma");
    cJSON_AddItemToArray(array, sref);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("delta_replaced");
    cJSON *target = cJSON_GetArrayItem(array, 1);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(array, target, replacement);
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = printed[1];
    buffer[2] = (char)('0' + (int)replaced);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
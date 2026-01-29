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
//<ID> 2019
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
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON *item2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, item1);
    cJSON_AddItemToArray(array, item2);
    cJSON *item_ref = cJSON_CreateStringReference("gamma");
    cJSON_AddItemToArray(array, item_ref);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("delta");
    cJSON_ReplaceItemViaPointer(array, item1, replacement);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0))[0];
    buffer[2] = cJSON_GetStringValue(cJSON_GetArrayItem(array, 1))[0];
    buffer[3] = cJSON_GetStringValue(cJSON_GetArrayItem(array, 2))[0];
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1976
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("original");
    cJSON *ref = cJSON_CreateStringReference("original_ref");

    // step 2: Configure
    cJSON_AddItemToObject(root, "item", orig);
    cJSON_AddItemToObject(root, "item_ref", ref);
    cJSON *replacement = cJSON_CreateString("replaced");

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(root, orig, replacement);
    char *printed = cJSON_Print(root);
    const char *val = cJSON_GetStringValue(replacement);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = val[0];
    buffer[1] = printed[0];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
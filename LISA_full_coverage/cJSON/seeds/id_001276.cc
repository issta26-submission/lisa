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
//<ID> 1276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *txt = cJSON_CreateString("example");
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(items, num);
    cJSON_AddItemToArray(items, txt);
    cJSON_AddItemToArray(items, nul);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);
    (void)first_invalid;
    char *printed = cJSON_Print(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    if (printed_len > 0) {
        memcpy(buffer, printed, printed_len + 1);
    }

    // step 4: Validate & Cleanup
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_invalid = cJSON_IsInvalid(second);
    (void)second_invalid;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1273
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
    cJSON *n = cJSON_CreateNumber(42.0);
    cJSON *s = cJSON_CreateString("example");
    cJSON *b = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, n);
    cJSON_AddItemToArray(items, s);
    cJSON_AddItemToArray(items, b);

    // step 3: Operate
    char *printed = cJSON_Print(root);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_invalid = cJSON_IsInvalid(second);
    (void)second_invalid;
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
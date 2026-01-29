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
//<ID> 2013
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
    cJSON *str1 = cJSON_CreateString("initial_value");
    cJSON *strref = cJSON_CreateStringReference("reference_value");
    cJSON_AddItemToArray(array, str1);
    cJSON_AddItemToArray(array, strref);
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_bool replace_result = cJSON_ReplaceItemViaPointer(array, str1, replacement);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    int printed_len = (int)strlen(printed);
    char *buffer = (char *)cJSON_malloc((size_t)32);
    memset(buffer, 0, (size_t)32);
    buffer[0] = printed[0];
    buffer[1] = first_str[0];
    buffer[2] = (char)('0' + (int)replace_result % 10);
    buffer[3] = (char)('0' + (printed_len % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
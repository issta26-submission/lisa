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
//<ID> 2018
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
    cJSON *item1 = cJSON_CreateString("one");
    cJSON *item2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(array, item1);
    cJSON_AddItemToArray(array, item2);

    // step 2: Configure
    cJSON *ref = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToArray(array, ref);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(array, item2, replacement);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    const int buflen = 32;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    int arr_size = cJSON_GetArraySize(array);
    buffer[0] = printed ? printed[0] : 'x';
    buffer[1] = (char)('0' + (arr_size % 10));
    buffer[2] = (char)('0' + (replaced_ok % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
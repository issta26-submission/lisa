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
//<ID> 2033
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
    cJSON *elem0 = cJSON_CreateString("hello");
    cJSON *elem1 = cJSON_CreateString("world");
    cJSON_AddItemToArray(array, elem0);
    cJSON_AddItemToArray(array, elem1);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    cJSON *replacement_false = cJSON_CreateFalse();
    cJSON_ReplaceItemInObjectCaseSensitive(root, "active", replacement_false);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *first_str = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0));
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = first_str ? first_str[0] : '\0';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
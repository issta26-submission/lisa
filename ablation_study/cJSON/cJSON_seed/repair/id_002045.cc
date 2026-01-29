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
//<ID> 2045
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
    cJSON_AddStringToObject(root, "label", "initial");
    cJSON_AddRawToObject(root, "raw", "{\"x\":1}");
    cJSON *elem0 = cJSON_CreateString("alpha");
    cJSON *elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, elem0);
    cJSON_AddItemToArray(array, elem1);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(array, 1);
    cJSON_SetValuestring(second, "bravo");
    cJSON *first = cJSON_GetArrayItem(array, 0);
    const char *first_str = cJSON_GetStringValue(first);
    const char *second_str = cJSON_GetStringValue(second);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = first_str[0];
    buffer[2] = second_str[0];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
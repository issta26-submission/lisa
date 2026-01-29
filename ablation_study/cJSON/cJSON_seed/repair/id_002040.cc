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
//<ID> 2040
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
    cJSON *label = cJSON_AddStringToObject(root, "label", "initial");

    // step 2: Configure
    cJSON *elem0 = cJSON_CreateString("elem0");
    cJSON_AddItemToArray(array, elem0);
    cJSON *elem1 = cJSON_CreateString("elem1");
    cJSON_AddItemToArray(array, elem1);
    cJSON_AddRawToObject(root, "raw", "{\"rawnum\":100}");
    cJSON *first = cJSON_GetArrayItem(array, 0);
    char *newval = cJSON_SetValuestring(first, "modified0");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    const char *s0 = cJSON_GetStringValue(first);
    const char *s1 = cJSON_GetStringValue(second);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = s0 ? s0[0] : '\0';
    buffer[2] = s1 ? s1[0] : '\0';
    buffer[3] = newval ? newval[0] : '\0';
    buffer[4] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
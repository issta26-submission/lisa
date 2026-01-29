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
//<ID> 2049
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
    cJSON_AddStringToObject(root, "label", "initial");
    cJSON *elem = cJSON_CreateString("elem0");
    cJSON_AddItemToArray(array, elem);

    // step 2: Configure
    const char raw_json[] = "{\"rawnum\":123}";
    cJSON_AddRawToObject(root, "raw", raw_json);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON_SetValuestring(first, "elem0_modified");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *strval = cJSON_GetStringValue(first);
    buffer[0] = printed[0];
    buffer[1] = strval[0];
    buffer[2] = raw_json[1];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
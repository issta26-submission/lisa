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
//<ID> 2048
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
    cJSON *label = cJSON_AddStringToObject(root, "label", "original");

    // step 2: Configure
    cJSON *elem0 = cJSON_CreateString("first");
    cJSON_AddItemToArray(array, elem0);
    cJSON *elem1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(array, elem1);
    cJSON *picked = cJSON_GetArrayItem(array, 1);
    char *set_ret = cJSON_SetValuestring(picked, "modified");
    (void)set_ret;
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw_blob", "{\"note\":123}");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = cJSON_GetStringValue(cJSON_GetArrayItem(array, 1))[0];
    buffer[2] = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"))[0];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
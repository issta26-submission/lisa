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
//<ID> 2038
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
    cJSON *name_item = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "name", name_item);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    cJSON *elem0 = cJSON_CreateString("first");
    cJSON *elem1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(array, elem0);
    cJSON_AddItemToArray(array, elem1);
    cJSON *replacement_name = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "name", replacement_name);

    // step 3: Operate and Validate
    char json_sample[] = " { \"alpha\" : \"beta\" } ";
    cJSON_Minify(json_sample);
    char *printed = cJSON_Print(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0))[0];
    buffer[2] = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"))[0];
    buffer[3] = json_sample[1];
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
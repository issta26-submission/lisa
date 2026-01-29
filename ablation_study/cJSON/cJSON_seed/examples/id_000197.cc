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
//<ID> 197
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
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToArray(array, raw);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(array, str_item);

    // step 3: Operate & Validate
    cJSON_bool raw_check = cJSON_IsRaw(raw);
    cJSON *got_array = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(got_array);
    cJSON *first = cJSON_GetArrayItem(got_array, 0);
    char *second_val = cJSON_GetStringValue(cJSON_GetArrayItem(got_array, 1));
    cJSON *echo = cJSON_CreateString(second_val);
    cJSON_AddItemToObject(root, "echo", echo);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}
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
//<ID> 495
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"fuzz\",\"value\":42}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();

    // step 2: Configure
    cJSON_AddItemToArray(array, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(2.5));
    cJSON_AddItemToArray(array, parsed);
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddItemToObject(settings, "list", array);

    // step 3: Operate
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToArray(extras, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(extras, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "extras", extras);

    // step 4: Validate and Cleanup
    cJSON *got_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *got_list = cJSON_GetObjectItem(got_settings, "list");
    int list_size = cJSON_GetArraySize(got_list);
    cJSON *value_item = cJSON_GetObjectItem(parsed, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    (void)list_size;
    (void)value_num;
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
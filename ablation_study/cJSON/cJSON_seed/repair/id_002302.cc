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
//<ID> 2302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddStringToObject(data, "id", "sensor-007");
    cJSON_AddNumberToObject(data, "value", 42.0);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(7.0));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "type", "example");
    cJSON_AddItemToObject(meta, "enabled", cJSON_CreateTrue());

    // step 3: Operate
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *id_item = cJSON_GetObjectItem(got_data, "id");
    cJSON_bool id_is_string = cJSON_IsString(id_item);
    char *id_str = cJSON_GetStringValue(id_item);
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    cJSON_bool list_is_array = cJSON_IsArray(got_list);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (char)('0' + (list_is_array != 0));
    buffer[2] = (char)((id_is_string && id_str && id_str[0]) ? id_str[0] : '\0');
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
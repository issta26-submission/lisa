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
//<ID> 2166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(data, entry1);
    cJSON *entry2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(data, entry2);
    cJSON *data_copy = cJSON_Duplicate(data, 1);
    cJSON_AddItemToObject(root, "data_copy", data_copy);

    // step 3: Operate and Validate
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON *valid_flag = cJSON_CreateBool(enabled_is_bool && name_is_string);
    cJSON_AddItemToObject(root, "valid", valid_flag);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json[0];
    buffer[1] = json[1];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1919
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON *version_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(config, "version", version_item);

    // step 2: Configure
    cJSON_SetValuestring(name_item, "updated_name");
    cJSON *new_version = cJSON_CreateNumber(2.718);
    cJSON_ReplaceItemInObject(config, "version", new_version);
    cJSON *extra_item = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(config, "extra", extra_item);

    // step 3: Operate and Validate
    cJSON *got_version = cJSON_GetObjectItem(config, "version");
    double version_value = cJSON_GetNumberValue(got_version);
    cJSON *got_name = cJSON_GetObjectItem(config, "name");
    char *name_value = cJSON_GetStringValue(got_name);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (((int)version_value) % 10));
    buffer[2] = name_value ? name_value[0] : 'X';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
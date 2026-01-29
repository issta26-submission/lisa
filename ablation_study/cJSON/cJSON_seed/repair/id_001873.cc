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
//<ID> 1873
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);

    // step 2: Configure
    cJSON *nested = cJSON_AddObjectToObject(config, "nested");
    cJSON *name_item = cJSON_GetObjectItem(meta, "name");
    cJSON_bool is_str = cJSON_IsString(name_item);
    cJSON_bool is_meta_obj = cJSON_IsObject(meta);
    char *set_ret = cJSON_SetValuestring(name_item, "sample");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = set_ret ? set_ret[0] : 'V';
    buffer[2] = (char)('0' + (is_str % 10));
    buffer[3] = (char)('0' + (is_meta_obj % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
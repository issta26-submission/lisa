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
//<ID> 1876
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", name);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    char *set_ret = cJSON_SetValuestring(name_item, "example_v2");
    cJSON *embedded = cJSON_AddObjectToObject(config, "embedded");
    cJSON_AddItemToObject(embedded, "inner_flag", cJSON_CreateTrue());

    // step 3: Operate and Validate
    cJSON *embedded_item = cJSON_GetObjectItem(config, "embedded");
    cJSON_bool is_obj = cJSON_IsObject(embedded_item);
    cJSON *name_check = cJSON_GetObjectItem(config, "name");
    cJSON_bool is_str = cJSON_IsString(name_check);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)is_obj);
    buffer[2] = (char)('0' + (int)is_str);
    buffer[3] = set_ret ? set_ret[0] : 'S';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
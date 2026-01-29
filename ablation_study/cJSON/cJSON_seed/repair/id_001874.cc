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
//<ID> 1874
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
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "enabled", enabled);

    // step 2: Configure
    cJSON *settings = cJSON_AddObjectToObject(meta, "settings");
    cJSON_AddItemToObject(settings, "option", cJSON_CreateNumber(7.0));
    char *set_ret = cJSON_SetValuestring(name_item, "updated");
    (void)set_ret;
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_bool name_is_str = cJSON_IsString(name_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    cJSON *enabled_check = cJSON_GetObjectItem(meta, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled_check);
    buffer[1] = (char)('0' + (meta_is_obj % 10));
    buffer[2] = (char)('0' + (name_is_str % 10));
    buffer[3] = (char)('0' + (enabled_is_true % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
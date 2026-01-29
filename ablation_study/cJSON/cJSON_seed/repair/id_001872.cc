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
//<ID> 1872
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
    cJSON *config = cJSON_AddObjectToObject(meta, "config");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *version = cJSON_CreateString("1.0");
    cJSON_AddItemToObject(config, "version", version);

    // step 2: Configure
    cJSON *ver_item = cJSON_GetObjectItem(config, "version");
    char *set_ret = cJSON_SetValuestring(ver_item, "2.0");
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool meta_is_object = cJSON_IsObject(meta);
    cJSON_bool ver_is_string = cJSON_IsString(ver_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t buf_len = 64;
    char *buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = set_ret ? set_ret[0] : 'V';
    buffer[2] = (char)('0' + (int)(meta_is_object % 10));
    buffer[3] = (char)('0' + (int)(ver_is_string % 10));
    buffer[4] = '\0';
    char *got_str = cJSON_GetStringValue(ver_item);
    (void)got_str;
    (void)enabled_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
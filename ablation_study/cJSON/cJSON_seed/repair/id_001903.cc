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
//<ID> 1903
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
    cJSON *name_added = cJSON_AddStringToObject(config, "name", "fuzzer");
    cJSON *false_added = cJSON_AddFalseToObject(config, "enabled");

    // step 2: Configure
    cJSON_bool has_enabled = cJSON_HasObjectItem(config, "enabled");
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    const char *name_str = cJSON_GetStringValue(name_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (has_enabled % 10));
    buffer[2] = (char)('0' + (enabled_is_bool % 10));
    buffer[3] = (char)('0' + ((name_str && name_str[0]) ? ((unsigned char)name_str[0] % 10) : ('X' % 10)));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
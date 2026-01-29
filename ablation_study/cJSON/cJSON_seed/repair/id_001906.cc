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
//<ID> 1906
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);
    cJSON *name_item = cJSON_AddStringToObject(container, "name", "example");
    cJSON *enabled_item = cJSON_AddFalseToObject(container, "enabled");

    // step 2: Configure
    cJSON *got_name = cJSON_GetObjectItem(container, "name");
    cJSON *got_enabled = cJSON_GetObjectItem(container, "enabled");
    cJSON_bool has_enabled = cJSON_HasObjectItem(container, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *name_str = cJSON_GetStringValue(got_name);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'X';
    buffer[2] = (char)('0' + (has_enabled % 10));
    buffer[3] = (char)('0' + (enabled_is_bool % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
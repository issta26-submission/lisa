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
//<ID> 1878
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *active_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(flags, "active", active_true);
    cJSON *name_str = cJSON_CreateString("example");
    cJSON_AddItemToObject(settings, "name", name_str);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(settings, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON *flags_item = cJSON_GetObjectItem(root, "flags");
    cJSON_bool flags_is_object = cJSON_IsObject(flags_item);
    cJSON *active_item = cJSON_GetObjectItem(flags, "active");
    cJSON_bool active_is_true = cJSON_IsTrue(active_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)(name_is_string % 10));
    buffer[2] = (char)('0' + (int)(flags_is_object % 10));
    buffer[3] = (char)('0' + (int)(active_is_true % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
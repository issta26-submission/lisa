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
//<ID> 1685
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example");
    cJSON *active_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "active", active_item);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddStringToObject(meta, "note", "initialized");

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON *got_missing = cJSON_GetObjectItem(root, "missing");
    cJSON_bool active_is_true = cJSON_IsTrue(got_active);
    cJSON_bool missing_is_invalid = cJSON_IsInvalid(got_missing);
    const char *name_str = cJSON_GetStringValue(got_name);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Y';
    buffer[2] = (char)('0' + (int)active_is_true);
    buffer[3] = (char)('0' + (int)missing_is_invalid);
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
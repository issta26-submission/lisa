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
//<ID> 1683
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_str = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_str);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "value", 42.0);

    // step 3: Operate and Validate
    cJSON *retr_name = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    cJSON *retr_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_true = cJSON_IsTrue(retr_active);
    cJSON *retr_missing = cJSON_GetObjectItem(root, "missing");
    cJSON_bool missing_invalid = cJSON_IsInvalid(retr_missing);
    char *printed = cJSON_Print(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)active_is_true);
    buffer[3] = (char)('0' + (int)missing_invalid);
    (void)buffer;
    (void)meta;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1692
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
    cJSON *name_item = cJSON_CreateString("tester");
    cJSON_AddItemToObject(settings, "name", name_item);
    cJSON *items = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, item0);
    cJSON *item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, item1);
    cJSON_AddItemToObject(settings, "items", items);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "env", "prod");
    cJSON_AddNumberToObject(meta, "build", 42.0);

    // step 3: Operate and Validate
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON *retr_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *retr_name = cJSON_GetObjectItem(retr_settings, "name");
    char *name_val = cJSON_GetStringValue(retr_name);
    cJSON *retr_items = cJSON_GetObjectItem(retr_settings, "items");
    cJSON_DeleteItemFromArray(retr_items, 0);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = name_val ? name_val[0] : 'X';
    buffer[1] = (char)('0' + (int)has_settings);
    char *env_val = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "env"));
    buffer[2] = env_val ? env_val[0] : 'Y';
    (void)buffer;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
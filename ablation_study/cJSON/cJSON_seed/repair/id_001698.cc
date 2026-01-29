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
//<ID> 1698
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
    cJSON_AddStringToObject(config, "version", "1.2.3");
    cJSON *items = cJSON_AddArrayToObject(config, "items");
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON *item1 = cJSON_CreateString("beta");
    cJSON *item2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, item0);
    cJSON_AddItemToArray(items, item1);
    cJSON_AddItemToArray(items, item2);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(config, "meta");
    cJSON_AddStringToObject(meta, "author", "tester");
    cJSON_AddNumberToObject(meta, "count", 3);

    // step 3: Operate and Validate
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_items = cJSON_GetObjectItem(got_config, "items");
    cJSON *first = cJSON_GetArrayItem(got_items, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON_DeleteItemFromArray(got_items, 1);
    char *out = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = out ? out[0] : 'X';
    buffer[1] = first_str ? first_str[0] : 'Y';
    buffer[2] = (char)('0' + (int)has_config);
    (void)buffer;
    (void)got_config;
    (void)got_items;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
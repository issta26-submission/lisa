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
//<ID> 1690
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
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "fuzz_test");
    cJSON_AddStringToObject(config, "version", "1.0");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *item0 = cJSON_CreateString("first");
    cJSON *item1 = cJSON_CreateString("second");
    cJSON *item2 = cJSON_CreateString("third");
    cJSON_AddItemToArray(items, item0);
    cJSON_AddItemToArray(items, item1);
    cJSON_AddItemToArray(items, item2);

    // step 2: Configure
    cJSON_AddStringToObject(config, "status", "active");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "count", 3);

    // step 3: Operate and Validate
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON *retr_name = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(retr_name);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = has_name ? 'Y' : 'N';
    buffer[2] = name_val ? name_val[0] : 'Z';
    cJSON_DeleteItemFromArray(items, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
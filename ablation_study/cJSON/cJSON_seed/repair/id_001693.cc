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
//<ID> 1693
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
    cJSON_AddStringToObject(root, "name", "example");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *item1 = cJSON_CreateString("first");
    cJSON *item2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(items, item1);
    cJSON_AddItemToArray(items, item2);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "note", "configured");
    cJSON *child = cJSON_AddObjectToObject(meta, "child");
    cJSON_AddStringToObject(child, "desc", "inner");

    // step 3: Operate and Validate
    cJSON_bool meta_exists = cJSON_HasObjectItem(root, "meta");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    char *printed = cJSON_Print(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)meta_exists);
    (void)retrieved_meta;
    (void)child;

    cJSON_DeleteItemFromArray(items, 0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
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
//<ID> 1695
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
    cJSON *name_str = cJSON_CreateString("fuzz");
    cJSON_AddItemToObject(root, "name", name_str);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *it1 = cJSON_CreateString("one");
    cJSON *it2 = cJSON_CreateString("two");
    cJSON *it3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(items, it1);
    cJSON_AddItemToArray(items, it2);
    cJSON_AddItemToArray(items, it3);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "status", "active");
    cJSON *child = cJSON_AddObjectToObject(meta, "child");
    cJSON_AddNumberToObject(child, "count", 3);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(items, 1);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_value = cJSON_GetStringValue(got_name);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_value ? name_value[0] : 'Y';
    buffer[2] = (char)('0' + (int)has_meta);
    (void)buffer;
    (void)child;
    (void)got_name;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
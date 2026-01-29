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
//<ID> 1691
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
    cJSON *name_item = cJSON_CreateString("fuzzer");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *it0 = cJSON_CreateString("one");
    cJSON *it1 = cJSON_CreateString("two");
    cJSON *it2 = cJSON_CreateString("three");
    cJSON_AddItemToArray(items, it0);
    cJSON_AddItemToArray(items, it1);
    cJSON_AddItemToArray(items, it2);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "status", "initialized");
    cJSON *sub = cJSON_AddObjectToObject(meta, "sub");
    cJSON_AddNumberToObject(sub, "count", 3);

    // step 3: Operate and Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *retr_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retr_name = cJSON_GetObjectItem(retr_meta, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    cJSON_DeleteItemFromArray(items, 1);
    char *printed = cJSON_Print(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)has_meta);
    (void)buffer;
    (void)retr_meta;
    (void)retr_name;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
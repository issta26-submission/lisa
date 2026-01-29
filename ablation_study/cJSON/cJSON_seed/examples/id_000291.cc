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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example_name");
    cJSON *version = cJSON_CreateNumber(1.23);
    cJSON *items = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10);
    cJSON *n2 = cJSON_CreateNumber(20);

    // step 2: Configure
    cJSON_AddItemToObjectCS(meta, "name", name);
    cJSON_AddItemToObjectCS(meta, "version", version);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "items", items);

    // step 3: Operate & Validate
    cJSON_bool has_meta_before = cJSON_HasObjectItem(root, "meta");
    cJSON *has_meta_flag = cJSON_CreateBool(has_meta_before);
    cJSON_AddItemToObjectCS(root, "has_meta_before", has_meta_flag);
    cJSON *meta_ptr = cJSON_GetObjectItem(root, "meta");
    cJSON *name_ptr = cJSON_GetObjectItem(meta_ptr, "name");
    const char *name_text = cJSON_GetStringValue(name_ptr);
    cJSON *name_copy = cJSON_CreateString(name_text);
    cJSON_AddItemToObjectCS(root, "name_copy", name_copy);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "meta");
    cJSON_bool has_meta_after = cJSON_HasObjectItem(root, "meta");
    cJSON *meta_deleted_flag = cJSON_CreateBool(has_meta_after);
    cJSON_AddItemToObjectCS(root, "meta_deleted", meta_deleted_flag);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
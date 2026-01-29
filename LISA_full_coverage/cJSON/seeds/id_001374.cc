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
//<ID> 1374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"nested\":{\"a\":1,\"b\":true},\"list\":[1,2,3]}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "num", num);
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greet", greet);

    // step 2: Configure
    cJSON *nested = cJSON_GetObjectItem(parsed, "nested");
    cJSON *dup_nested = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "dup_nested", dup_nested);
    cJSON *list = cJSON_GetObjectItem(parsed, "list");
    cJSON *list_ref = cJSON_CreateArrayReference(list);
    cJSON_AddItemToObject(root, "list_ref", list_ref);

    // step 3: Operate
    cJSON *new_greet = cJSON_CreateString("world");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "greet", new_greet);
    cJSON_bool has_num = cJSON_HasObjectItem(root, "num");
    cJSON *has_num_item = cJSON_CreateBool(has_num);
    cJSON_AddItemToObject(root, "has_num", has_num_item);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
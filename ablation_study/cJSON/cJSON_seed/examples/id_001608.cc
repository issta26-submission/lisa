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
//<ID> 1608
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first = cJSON_CreateString("first_item");
    cJSON_AddItemToArray(items, first);
    const char *embedded_raw = "{\"embedded\":true}";
    cJSON *raw_item = cJSON_CreateRaw(embedded_raw);
    cJSON_AddItemToArray(items, raw_item);
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, num_item);

    // step 2: Configure
    cJSON *has_items_flag = cJSON_CreateNumber((double)cJSON_HasObjectItem(root, "items"));
    cJSON_AddItemToObject(root, "has_items", has_items_flag);
    int items_count = cJSON_GetArraySize(items);
    cJSON *items_count_num = cJSON_CreateNumber((double)items_count);
    cJSON_AddItemToObject(root, "items_count", items_count_num);
    cJSON *config_is_obj = cJSON_CreateNumber((double)cJSON_IsObject(config));
    cJSON_AddItemToObject(root, "config_is_object", config_is_obj);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
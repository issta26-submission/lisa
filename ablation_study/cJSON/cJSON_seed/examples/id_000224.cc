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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first_num = cJSON_CreateNumber(42.0);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, first_num);
    cJSON_AddItemToArray(items, greeting);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("1.0"));
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_AddItemToObject(root, "has_items", cJSON_CreateNumber((double)has_items));

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(items);
    cJSON_AddItemToObject(root, "items_count", cJSON_CreateNumber((double)count));
    char *buffer = (char*)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, 512, 0);
    cJSON_AddItemToObject(root, "printed_ok", cJSON_CreateNumber((double)printed_ok));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
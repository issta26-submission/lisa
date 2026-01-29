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
//<ID> 2291
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
    cJSON *item_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, item_false);
    cJSON *item_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, item_num);
    cJSON *item_str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, item_str);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(items));
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_AddItemToObject(root, "has_items", cJSON_CreateBool(has_items));

    // step 3: Operate
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = has_items ? '1' : '0';

    // step 4: Validate and Cleanup
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
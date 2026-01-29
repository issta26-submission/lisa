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
//<ID> 1719
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
    cJSON *true_node = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true_node);
    cJSON *str_node = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, str_node);
    cJSON *num_node = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(items, num_node);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "flag", cJSON_CreateTrue());
    cJSON_AddStringToObject(meta, "desc", "sequence test");

    // step 3: Operate and Validate
    cJSON *retr_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_array_flag = cJSON_IsArray(retr_items);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = is_array_flag ? 'A' : 'N';
    cJSON *retr_meta = cJSON_GetObjectItem(root, "meta");
    const char *desc_val = cJSON_GetStringValue(cJSON_GetObjectItem(retr_meta, "desc"));
    buffer[2] = desc_val ? desc_val[0] : 'Z';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
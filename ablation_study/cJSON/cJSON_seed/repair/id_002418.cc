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
//<ID> 2418
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
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("second");
    cJSON_AddItemToArray(items, num_item);
    cJSON_AddItemToArray(items, str_item);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_AddStringToObject(root, "meta", "initial_meta");

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];

    // step 3: Operate
    cJSON *dup_items = cJSON_GetObjectItem(dup_root, "items");
    cJSON *second = cJSON_GetArrayItem(dup_items, 1);
    char *second_val = cJSON_GetStringValue(second);
    buffer[1] = second_val[0];

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(dup_root);
    buffer[2] = printed2[0];
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}
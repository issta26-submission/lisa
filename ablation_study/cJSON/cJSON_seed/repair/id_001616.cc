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
//<ID> 1616
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *s1 = cJSON_CreateString("second");
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "id", 7.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "name", "example");

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    char *second_str = cJSON_GetStringValue(second_item);
    cJSON *third_item = cJSON_GetArrayItem(items, 2);
    double third_val = cJSON_GetNumberValue(third_item);
    cJSON *meta_id_item = cJSON_GetObjectItemCaseSensitive(meta, "id");
    double meta_id = cJSON_GetNumberValue(meta_id_item);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = second_str[0];
    buffer[1] = (char)('0' + ((int)meta_id % 10));
    buffer[2] = (char)('0' + ((int)third_val % 10));

    cJSON_DeleteItemFromArray(items, 0);
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");

    // step 4: Cleanup
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
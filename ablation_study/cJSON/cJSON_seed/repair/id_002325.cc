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
//<ID> 2325
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
    cJSON *ref_str = cJSON_CreateStringReference("ref-string");
    cJSON_AddItemToArray(items, ref_str);
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(items, num_item);

    // step 2: Configure
    cJSON_AddStringToObject(root, "owner", "tester");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "scale", 2.0);
    cJSON_AddStringToObject(meta, "type", "dataset");

    // step 3: Operate
    cJSON *items_fetched = cJSON_GetObjectItem(root, "items");
    cJSON *items_dup = cJSON_Duplicate(items_fetched, 1);
    cJSON_AddItemToObject(root, "items_copy", items_dup);
    char *json = cJSON_PrintUnformatted(root);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    double second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 1));

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (first_str && first_str[0]) ? first_str[0] : '\0';
    buffer[2] = (char)('0' + ((int)second_val % 10));
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
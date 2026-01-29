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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *meta_nested = (cJSON *)0;
    cJSON *tmp_num = (cJSON *)0;
    cJSON *retrieved_raw = (cJSON *)0;
    char *json_unformatted = (char *)0;
    char *raw_string = (char *)0;
    cJSON_bool items_is_array = (cJSON_bool)0;
    double first_number = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    items = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);
    raw_item = cJSON_AddRawToObject(root, "raw_payload", "{\"status\":\"ok\",\"values\":[42,7]}");
    meta_nested = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "nested", meta_nested);
    tmp_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, tmp_num);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(7.0));

    // step 4: Operate
    items_is_array = cJSON_IsArray(cJSON_GetObjectItem(root, "items"));
    json_unformatted = cJSON_PrintUnformatted(root);
    retrieved_raw = cJSON_GetObjectItem(root, "raw_payload");
    raw_string = cJSON_GetStringValue(retrieved_raw);
    first_number = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));

    // step 5: Validate
    (void)items_is_array;
    (void)raw_string;
    (void)first_number;
    cJSON_GetObjectItem(meta, "nested");
    cJSON_GetObjectItem(root, "items");

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
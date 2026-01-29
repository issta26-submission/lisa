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
//<ID> 140
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional_null);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(meta, "count", count);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *it1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, it1);
    cJSON *it2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, it2);
    cJSON *it3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, it3);

    // step 3: Operate & Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    (void)has_meta;
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(got_meta);
    (void)meta_is_obj;
    int size = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_num = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
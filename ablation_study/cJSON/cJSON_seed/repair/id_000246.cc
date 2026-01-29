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
//<ID> 246
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
    cJSON *n0 = cJSON_CreateNumber(10.5);
    cJSON *n1 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddNumberToObject(root, "total", 2.0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *sum = cJSON_CreateNumber(30.75);
    cJSON_AddItemToObject(meta, "sum", sum);
    cJSON *avg = cJSON_CreateNumber(15.375);
    cJSON_AddItemToObject(meta, "avg", avg);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_AddNumberToObject(root, "first_value_extracted", first_val);
    cJSON_AddNumberToObject(root, "has_items_flag", (double)has_items);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
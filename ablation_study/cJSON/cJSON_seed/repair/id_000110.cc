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
//<ID> 110
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, num1);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, num2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "initial", 7.0);

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_backup", dup_items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *initial_obj = cJSON_GetObjectItem(meta, "initial");

    // step 3: Operate and Validate
    double v_first = cJSON_GetNumberValue(first_item);
    double v_dup_second = cJSON_GetNumberValue(cJSON_GetArrayItem(dup_items, 1));
    double v_initial = cJSON_GetNumberValue(initial_obj);
    double total = v_first + v_dup_second + v_initial;
    cJSON_AddNumberToObject(root, "combined_total", total);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}
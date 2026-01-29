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
//<ID> 908
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

    // step 2: Configure
    cJSON *active_item = cJSON_AddTrueToObject(meta, "active");
    const char *initial_name = "initial_name";
    cJSON *name_ref = cJSON_CreateStringReference(initial_name);
    cJSON_AddItemToObject(meta, "name", name_ref);
    double numbers[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *darray = cJSON_CreateDoubleArray(numbers, 4);
    cJSON_AddItemToObject(meta, "values", darray);

    // step 3: Operate
    cJSON *val0 = cJSON_GetArrayItem(darray, 0);
    cJSON *val1 = cJSON_GetArrayItem(darray, 1);
    double n0 = cJSON_GetNumberValue(val0);
    double n1 = cJSON_GetNumberValue(val1);
    double total = n0 + n1;
    cJSON_AddNumberToObject(meta, "sum_first_two", total);
    cJSON *new_name_ref = cJSON_CreateStringReference("renamed");
    cJSON_bool rep_ok = cJSON_ReplaceItemInObjectCaseSensitive(meta, "name", new_name_ref);
    (void)rep_ok;
    (void)active_item;

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
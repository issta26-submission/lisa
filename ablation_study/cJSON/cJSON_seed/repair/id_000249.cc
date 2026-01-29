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
//<ID> 249
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *n3 = cJSON_CreateNumber(-3.14);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "source", "generated");

    // step 3: Operate
    cJSON *second_item = cJSON_GetArrayItem(values, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *second_added = cJSON_AddNumberToObject(root, "second", second_val);
    cJSON_bool has_vals = cJSON_HasObjectItem(root, "values");
    cJSON_AddNumberToObject(root, "has_values", (double)has_vals);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)second_added;
    return 66;
}
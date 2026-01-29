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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_str[] = "{\"initial\": 10.5, \"name\": \"test\"}";
    cJSON *root = cJSON_Parse(json_str);

    // step 2: Configure
    double arr_vals[3] = {1.5, 2.5, 3.5};
    cJSON *darray = cJSON_CreateDoubleArray(arr_vals, 3);
    cJSON_bool added_darray = cJSON_AddItemToObjectCS(root, "doubles", darray);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_bool added_false = cJSON_AddItemToObjectCS(root, "enabled", false_item);

    // step 3: Operate & Validate
    cJSON_bool has_initial = cJSON_HasObjectItem(root, "initial");
    cJSON *initial_item = cJSON_GetObjectItem(root, "initial");
    double initial_val = cJSON_GetNumberValue(initial_item);
    double new_val = initial_val + (has_initial ? 5.0 : 0.0);
    cJSON *newnum = cJSON_CreateNumber(new_val);
    cJSON_AddItemToObjectCS(root, "initial_plus", newnum);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    (void)added_darray;
    (void)added_false;
    (void)has_initial;

    // API sequence test completed successfully
    return 66;
}
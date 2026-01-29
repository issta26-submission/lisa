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
//<ID> 1217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"vals\":[1,2,3],\"remove_me\":7}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "vals");
    cJSON *arr_ref = cJSON_CreateArrayReference(parsed_arr);
    cJSON_AddItemToObject(root, "vals_ref", arr_ref);
    cJSON *rm_item = cJSON_GetObjectItem(parsed, "remove_me");
    double doubled_val = cJSON_GetNumberValue(rm_item) * 2.0;
    cJSON *dnum = cJSON_CreateNumber(doubled_val);
    cJSON_AddItemToObject(root, "doubled", dnum);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObject(parsed, "remove_me");
    cJSON *first_item = cJSON_GetArrayItem(parsed_arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first", first_val);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "vals_ref");
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
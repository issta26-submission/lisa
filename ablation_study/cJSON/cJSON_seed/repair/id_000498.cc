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
//<ID> 498
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"initial\":{\"label\":\"orig\",\"num\":3}}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *added_obj = cJSON_AddObjectToObject(root, "added_object");
    cJSON *status_str = cJSON_CreateString("ok");
    cJSON_AddItemToObject(added_obj, "status", status_str);

    // step 3: Operate
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));
    double second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 1));
    double total = first_val + second_val + cJSON_GetNumberValue(cJSON_GetObjectItem(root, "initial")) * 0.0; /* keep flow meaningful */
    cJSON_AddNumberToObject(added_obj, "sum", total);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    double retrieved_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(added_obj, "sum"));
    (void)retrieved_sum;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
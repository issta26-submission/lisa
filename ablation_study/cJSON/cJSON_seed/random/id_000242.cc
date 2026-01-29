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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *manual_arr = (cJSON *)0;
    cJSON *auto_arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *got_array = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON *dup_item = (cJSON *)0;
    cJSON *topnum_item = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();

    // step 3: Configure
    manual_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "manual", manual_arr);
    auto_arr = cJSON_AddArrayToObject(root, "auto");
    cJSON_AddNumberToObject(root, "top_number", 42.0);
    num1 = cJSON_CreateNumber(100.0);
    num2 = cJSON_CreateNumber(200.0);
    cJSON_AddItemToArray(manual_arr, num1);
    cJSON_AddItemToArray(auto_arr, num2);

    // step 4: Operate
    got_array = cJSON_GetObjectItem(root, "auto");
    got_item = cJSON_GetArrayItem(got_array, 0);
    retrieved_value = cJSON_GetNumberValue(got_item);
    cJSON_AddNumberToObject(root, "auto_first_value", retrieved_value);
    dup_item = cJSON_Duplicate(got_item, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "copied_item", dup_item);
    topnum_item = cJSON_GetObjectItem(root, "top_number");

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
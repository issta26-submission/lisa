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
//<ID> 590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    char *name_value = (char *)0;
    int arr_size = 0;
    double first_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(42.0);
    num1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("example"));

    // step 3: Configure
    cJSON_AddNumberToObject(root, "static_value", 1.234);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(got_arr);
    first = cJSON_GetArrayItem(got_arr, 0);
    first_val = cJSON_GetNumberValue(first);
    added_num = cJSON_AddNumberToObject(root, "from_array", first_val);
    name_item = cJSON_GetObjectItem(root, "label");
    name_value = cJSON_GetStringValue(name_item);

    // step 5: Validate
    (void)arr_size;
    (void)first_val;
    (void)added_num;
    (void)name_value;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
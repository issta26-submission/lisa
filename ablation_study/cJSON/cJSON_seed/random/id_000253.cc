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
//<ID> 253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_array = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *extra_number = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON_bool add_ok_array = (cJSON_bool)0;
    cJSON_bool add_ok_label = (cJSON_bool)0;
    cJSON_bool add_ok_extra = (cJSON_bool)0;
    int numbers[4] = {1, 2, 3, 4};
    int array_size = 0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_array = cJSON_CreateIntArray(numbers, 4);
    add_ok_array = cJSON_AddItemToObject(root, "ints", int_array);
    label_item = cJSON_AddStringToObject(root, "label", "int_sequence");

    // step 3: Configure
    extra_number = cJSON_CreateNumber(10.5);
    add_ok_extra = cJSON_AddItemToObject(root, "extra", extra_number);

    // step 4: Operate
    array_size = cJSON_GetArraySize(int_array);
    count_item = cJSON_CreateNumber((double)array_size);
    add_ok_label = cJSON_AddItemToObject(root, "count", count_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
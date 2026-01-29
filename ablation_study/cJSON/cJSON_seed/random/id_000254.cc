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
//<ID> 254
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *status_str = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_count = (cJSON_bool)0;
    char *printed = (char *)0;
    int numbers[5] = {10, 20, 30, 40, 50};
    int arr_size = 0;
    double count_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(numbers, 5);
    add_ok_arr = cJSON_AddItemToObject(root, "ints", int_arr);

    // step 3: Configure
    status_str = cJSON_AddStringToObject(root, "status", "ok");
    arr_size = cJSON_GetArraySize(int_arr);
    count_item = cJSON_CreateNumber((double)arr_size);
    add_ok_count = cJSON_AddItemToObject(root, "count", count_item);

    // step 4: Operate
    retrieved_count = cJSON_GetObjectItem(root, "count");
    count_val = cJSON_GetNumberValue(retrieved_count);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 5: Validate
    (void)count_val;
    (void)add_ok_arr;
    (void)add_ok_count;
    (void)status_str;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
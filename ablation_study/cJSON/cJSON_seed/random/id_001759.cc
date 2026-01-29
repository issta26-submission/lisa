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
//<ID> 1759
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
    cJSON *num = (cJSON *)0;
    cJSON *detached_numbers = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    int arr_size = 0;
    double answer_value = 0.0;
    int validation = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int values[] = {10, 20, 30};
    int_arr = cJSON_CreateIntArray(values, 3);
    num = cJSON_CreateNumber(7.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "ints", int_arr);
    add_ok2 = cJSON_AddItemToObject(root, "num", num);

    // step 4: Operate
    detached_numbers = cJSON_DetachItemFromObject(root, "ints");
    arr_size = cJSON_GetArraySize(detached_numbers);
    retrieved_num = cJSON_GetObjectItem(root, "num");
    answer_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)(answer_value == 7.0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_numbers);
    (void)root;
    (void)int_arr;
    (void)num;
    (void)detached_numbers;
    (void)retrieved_num;
    (void)arr_size;
    (void)answer_value;
    (void)add_ok1;
    (void)add_ok2;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
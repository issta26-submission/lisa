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
//<ID> 1832
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
    cJSON *num2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    double detached_val = 0.0;
    int arr_size = 0;
    int validation = 0;
    char json_buf[] = " { \"alpha\" :  1 , \"beta\" : 2 } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.0);
    num2 = cJSON_CreateNumber(30.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    detached_val = (detached != (cJSON *)0) ? cJSON_GetNumberValue(detached) : 0.0;
    added_num = cJSON_AddNumberToObject(root, "extracted", detached_val);
    cJSON_AddNumberToObject(root, "api_added", 99.5);
    cJSON_Minify(json_buf);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(retrieved_arr);
    validation = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(detached != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(added_num != (cJSON *)0) + (int)(arr_size == 2) + (int)(json_buf[0] == '{');
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)num0;
    (void)num1;
    (void)num2;
    (void)detached;
    (void)retrieved_arr;
    (void)added_num;
    (void)detached_val;
    (void)arr_size;
    (void)json_buf;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
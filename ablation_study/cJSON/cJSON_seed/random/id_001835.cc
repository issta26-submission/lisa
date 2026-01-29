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
//<ID> 1835
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
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    char json_buf[] = " { \"pi\" : 3.14159, \"values\" : [ 1, 2 ] } ";
    int arr_size = 0;
    double detached_val = 0.0;
    double pi_val = 0.0;
    int validation = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    add_ok = cJSON_AddItemToArray(arr, num0);
    add_ok = cJSON_AddItemToArray(arr, num1);
    add_ok = cJSON_AddItemToObject(root, "values", arr);
    retrieved_pi = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 3: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    detached = cJSON_DetachItemFromArray(retrieved_arr, 0);
    detached_val = cJSON_GetNumberValue(detached);
    retrieved_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(retrieved_pi);
    cJSON_Minify(json_buf);

    // step 4: Validate
    validation = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(arr_size == 2) + (int)(detached != (cJSON *)0) + (int)(detached_val == 1.0) + (int)(retrieved_pi != (cJSON *)0) + (int)(pi_val == 3.14159);
    (void)validation;

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);
    (void)num0;
    (void)num1;
    (void)add_ok;
    (void)arr_size;
    (void)detached_val;
    (void)pi_val;
    (void)json_buf;
    (void)retrieved_arr;
    (void)retrieved_pi;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
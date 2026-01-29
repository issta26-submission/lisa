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
//<ID> 1833
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
    cJSON *num_alpha = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    double pi_val = 0.0;
    double detached_val = 0.0;
    int arr_size = 0;
    int validation = 0;
    char jsonbuf[] = " { \"x\" : 1 , \"y\" : 2 } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_alpha = cJSON_AddNumberToObject(root, "alpha", 1.0);
    num_pi = cJSON_AddNumberToObject(root, "pi", 3.14159);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(30));

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    retrieved_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(retrieved_pi);
    detached_item = cJSON_DetachItemFromArray(arr, 1);
    cJSON_Minify(jsonbuf);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(retrieved_arr);
    detached_val = cJSON_GetNumberValue(detached_item);
    validation = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(arr_size == 2) + (int)(detached_item != (cJSON *)0) + (int)(pi_val == 3.14159) + (int)(detached_val == 20.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    (void)num_alpha;
    (void)num_pi;
    (void)retrieved_pi;
    (void)pi_val;
    (void)detached_val;
    (void)arr_size;
    (void)jsonbuf;
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1838
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    char jsonbuf[128] = "{ \"x\" : 1 , \"y\" : [ 2 , 3 ] }";
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "numbers", arr);
    pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    detached_item = cJSON_DetachItemFromArray(retrieved_arr, 1);
    cJSON_Minify(jsonbuf);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(detached_item != (cJSON *)0) + (int)(pi_item != (cJSON *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_item);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)retrieved_arr;
    (void)pi_item;
    (void)jsonbuf;
    // API sequence test completed successfully
    return 66;
}
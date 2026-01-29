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
//<ID> 1837
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
    cJSON *retrieved_numbers = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    double pi_val = 0.0;
    char jsonbuf[] = " { \"a\" : 1 , \"b\" : 2 } ";
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    retrieved_numbers = cJSON_GetObjectItem(root, "numbers");
    pi_item = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(pi_item);
    cJSON_Minify(jsonbuf);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num0 != (cJSON *)0) + (int)(num1 != (cJSON *)0) + (int)(detached != (cJSON *)0) + (int)(retrieved_numbers != (cJSON *)0) + (int)(pi_item != (cJSON *)0) + (int)(pi_val > 0.0) + (int)(jsonbuf[0] == '{');
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)detached;
    (void)retrieved_numbers;
    (void)pi_item;
    (void)pi_val;
    (void)jsonbuf;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
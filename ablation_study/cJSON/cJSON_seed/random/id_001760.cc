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
//<ID> 1760
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool is_array = 0;
    double extracted_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    num = cJSON_CreateNumber(42.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s1);
    add_ok2 = cJSON_AddItemToArray(arr, s2);
    add_ok3 = cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "value", num);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "data");
    is_array = cJSON_IsArray(retrieved_arr);
    retrieved_num = cJSON_GetObjectItem(root, "value");
    extracted_value = cJSON_GetNumberValue(retrieved_num);
    cJSON_DeleteItemFromObject(root, "value");

    // step 5: Validate
    validation = (int)(is_array != 0) + (int)(extracted_value == 42.0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok3 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s1;
    (void)s2;
    (void)num;
    (void)retrieved_arr;
    (void)retrieved_num;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)is_array;
    (void)extracted_value;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
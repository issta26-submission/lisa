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
//<ID> 249
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
    cJSON *extra_num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_item0 = (cJSON *)0;
    cJSON *retrieved_item1 = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    double pi_val = 0.0;
    double sum = 0.0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");
    cJSON_AddNumberToObject(root, "pi", 3.1415926);

    // step 3: Configure
    num0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    extra_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "extra_number", extra_num);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    retrieved_item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    val0 = cJSON_GetNumberValue(retrieved_item0);
    retrieved_item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    val1 = cJSON_GetNumberValue(retrieved_item1);
    retrieved_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(retrieved_pi);
    sum = val0 + val1 + pi_val;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1740
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_answer = (cJSON *)0;
    cJSON *retrieved_answer_cs = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool is_number = 0;
    cJSON_bool is_number_cs = 0;
    int validation = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("first");
    str2 = cJSON_CreateString("second");
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToArray(arr, num_item);
    add_ok = cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "answer", cJSON_CreateNumber(42.0));

    // step 4: Operate
    retrieved_answer = cJSON_GetObjectItem(root, "answer");
    retrieved_answer_cs = cJSON_GetObjectItemCaseSensitive(root, "Answer");
    is_number = cJSON_IsNumber(retrieved_answer);
    is_number_cs = cJSON_IsNumber(retrieved_answer_cs);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation = (int)(is_number != 0) + (int)(arr_size == 3) + (int)(is_number_cs == 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)str1;
    (void)str2;
    (void)num_item;
    (void)retrieved_answer;
    (void)retrieved_answer_cs;
    (void)arr_size;
    (void)is_number;
    (void)is_number_cs;
    (void)add_ok;
    (void)validation;
    return 66;
    // API sequence test completed successfully
}
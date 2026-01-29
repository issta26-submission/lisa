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
//<ID> 910
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *title_key = "title";
    const char *count_key = "count";
    const char *title_str = "example";
    double val0 = 10.0;
    double val1 = 20.5;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool added_title = (cJSON_bool)0;
    cJSON_bool added_count = (cJSON_bool)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_title = (cJSON *)0;
    cJSON_bool invalid_check = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(val0);
    num1 = cJSON_CreateNumber(val1);

    // step 3: Configure - populate array and attach to root; add title and count via helpers
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);
    added_title = (cJSON_bool)(cJSON_AddStringToObject(root, title_key, title_str) != (cJSON *)0);
    added_count = (cJSON_bool)(cJSON_AddNumberToObject(root, count_key, 2.0) != (cJSON *)0);

    // step 4: Operate - retrieve items and perform an invalidity check
    retrieved_arr = cJSON_GetObjectItem(root, arr_key);
    retrieved_title = cJSON_GetObjectItem(root, title_key);
    invalid_check = cJSON_IsInvalid(retrieved_arr);

    // step 5: Validate - aggregate simple checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)added_arr + (int)added_title + (int)added_count + (int)(retrieved_arr != (cJSON *)0) + (int)(retrieved_title != (cJSON *)0) + (int)(invalid_check == 0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1525
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *item_false = (cJSON *)0;
    cJSON *num_obj = (cJSON *)0;
    cJSON *retr_flag = (cJSON *)0;
    cJSON *retr_count = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_flag = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool is_false = 0;
    double count_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.23);
    num2 = cJSON_CreateNumber(4.56);
    item_false = cJSON_CreateFalse();

    // step 3: Configure
    added_flag = cJSON_AddItemToObject(root, "flag", item_false);
    num_obj = cJSON_AddNumberToObject(root, "count", 42.0);
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retr_flag = cJSON_GetObjectItem(root, "flag");
    retr_count = cJSON_GetObjectItem(root, "count");
    is_false = cJSON_IsFalse(retr_flag);
    count_val = cJSON_GetNumberValue(retr_count);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_flag + (int)added_arr + (int)added_num1 + (int)added_num2 + (int)(num_obj != (cJSON *)0) + (int)is_false + (int)(count_val == 42.0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retr_flag;
    (void)retr_count;
    (void)count_val;
    (void)num1;
    (void)num2;
    (void)item_false;
    // API sequence test completed successfully
    return 66;
}
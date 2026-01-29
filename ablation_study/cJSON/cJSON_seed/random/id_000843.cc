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
//<ID> 843
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
    cJSON *retrieved_num = (cJSON *)0;
    char *printed = (char *)0;
    double extracted_value = 0.0;
    cJSON_bool added_s1 = 0;
    cJSON_bool added_s2 = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_num_to_root = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("first");
    s2 = cJSON_CreateString("second");
    num = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    added_s1 = cJSON_AddItemToArray(arr, s1);
    added_s2 = cJSON_AddItemToArray(arr, s2);
    added_arr_to_root = cJSON_AddItemToObject(root, "letters", arr);
    added_num_to_root = cJSON_AddItemToObject(root, "pi", num);

    // step 4: Operate
    retrieved_num = cJSON_GetObjectItem(root, "pi");
    extracted_value = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (retrieved_num != (cJSON *)0) + (printed != (char *)0) + (int)added_s1 + (int)added_s2 + (int)added_arr_to_root + (int)added_num_to_root + (extracted_value > 0.0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
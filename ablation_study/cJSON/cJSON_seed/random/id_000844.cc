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
//<ID> 844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    char *printed = (char *)0;
    double pi_value = 0.0;
    cJSON_bool added_num = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_s1 = 0;
    cJSON_bool added_s2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_pi = cJSON_CreateNumber(3.14159);
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("hello");
    s2 = cJSON_CreateString("world");

    // step 3: Configure
    added_num = cJSON_AddItemToObject(root, "pi", num_pi);
    added_arr = cJSON_AddItemToObject(root, "words", arr);
    added_s1 = cJSON_AddItemToArray(arr, s1);
    added_s2 = cJSON_AddItemToArray(arr, s2);

    // step 4: Operate
    retrieved_num = cJSON_GetObjectItem(root, "pi");
    retrieved_arr = cJSON_GetObjectItem(root, "words");
    pi_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    printed = cJSON_Print(root);
    validation_score = (int)added_num + (int)added_arr + (int)added_s1 + (int)added_s2 + (retrieved_num != (cJSON *)0) + (retrieved_arr != (cJSON *)0) + (printed != (char *)0) + (pi_value > 3.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
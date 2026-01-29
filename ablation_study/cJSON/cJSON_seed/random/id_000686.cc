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
//<ID> 686
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
    cJSON *label = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_label = 0;
    int arr_size = 0;
    int validity_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(7.0);
    label = cJSON_CreateString("example");

    // step 3: Configure
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_label = cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    validity_score = (printed != (char *)0) + (arr_size == 2) + (added_num1 != 0) + (added_num2 != 0) + (added_label != 0);
    (void)validity_score;
    (void)added_num1;
    (void)added_num2;
    (void)added_label;
    (void)arr_size;
    (void)num1;
    (void)num2;
    (void)label;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
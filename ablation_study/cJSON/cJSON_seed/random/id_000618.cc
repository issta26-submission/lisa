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
//<ID> 618
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
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *str_ref = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    cJSON *adjusted_num = (cJSON *)0;
    cJSON *retr_adjusted = (cJSON *)0;
    const char *ref_text = "hello_reference";
    double base_value = 10.0;
    double adjusted_value = 0.0;
    cJSON_bool flag_is_false = 0;
    double check_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.14159);
    false_item = cJSON_CreateFalse();
    str_ref = cJSON_CreateStringReference(ref_text);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "flag", false_item);
    added_num = cJSON_AddNumberToObject(root, "base", base_value);

    // step 4: Operate
    flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    adjusted_value = base_value + (double)flag_is_false;
    adjusted_num = cJSON_AddNumberToObject(root, "adjusted", adjusted_value);
    cJSON_AddItemReferenceToObject(root, "greeting", str_ref);

    // step 5: Validate
    retr_adjusted = cJSON_GetObjectItem(root, "adjusted");
    check_value = cJSON_GetNumberValue(retr_adjusted);
    (void)check_value;
    (void)added_num;
    (void)adjusted_num;
    (void)arr;
    (void)num_item;
    (void)str_ref;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
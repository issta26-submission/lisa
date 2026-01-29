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
//<ID> 1040
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *label_text = "label_value";
    const char *array_key = "numbers";
    const char *label_key = "label";
    cJSON *root = (cJSON *)0;
    cJSON *numbers = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *fetched_numbers = (cJSON *)0;
    cJSON *fetched_num0 = (cJSON *)0;
    int array_size = 0;
    double val0 = 0.0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_label = (cJSON_bool)0;
    cJSON_bool add_ok_num0 = (cJSON_bool)0;
    cJSON_bool add_ok_num1 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array and items to insert
    root = cJSON_CreateObject();
    numbers = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.5);
    str_item = cJSON_CreateString(label_text);

    // step 3: Configure - attach array and label into the root object and attach numbers into the array
    add_ok_arr = cJSON_AddItemToObjectCS(root, array_key, numbers);
    add_ok_label = cJSON_AddItemToObjectCS(root, label_key, str_item);
    add_ok_num0 = cJSON_AddItemToArray(numbers, num0);
    add_ok_num1 = cJSON_AddItemToArray(numbers, num1);

    // step 4: Operate - fetch the array and a number, then extract size and numeric value
    fetched_numbers = cJSON_GetObjectItem(root, array_key);
    array_size = cJSON_GetArraySize(fetched_numbers);
    fetched_num0 = cJSON_GetArrayItem(fetched_numbers, 0);
    val0 = cJSON_GetNumberValue(fetched_num0);

    // step 5: Validate - produce a small validation score exercising used APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(numbers != (cJSON *)0) + (int)(num0 != (cJSON *)0) + (int)(num1 != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_label) + (int)(add_ok_num0) + (int)(add_ok_num1) + (int)(array_size == 2) + (int)(val0 == 10.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
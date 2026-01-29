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
//<ID> 1046
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *nums = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *num3 = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    int array_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    cJSON_bool add_ok_array = (cJSON_bool)0;
    cJSON_bool add_ok_string = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create an object, an array and some items
    root = cJSON_CreateObject();
    nums = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    num3 = cJSON_CreateNumber(3.0);
    greeting = cJSON_CreateString("hello");

    // step 3: Configure - populate the array and attach items to the root object using CS add
    cJSON_AddItemToArray(nums, num1);
    cJSON_AddItemToArray(nums, num2);
    cJSON_AddItemToArray(nums, num3);
    add_ok_string = cJSON_AddItemToObjectCS(root, "greeting", greeting);
    add_ok_array = cJSON_AddItemToObjectCS(root, "nums", nums);

    // step 4: Operate - inspect the array and extract numeric values
    array_size = cJSON_GetArraySize(nums);
    fetched0 = cJSON_GetArrayItem(nums, 0);
    fetched1 = cJSON_GetArrayItem(nums, 1);
    val0 = cJSON_GetNumberValue(fetched0);
    val1 = cJSON_GetNumberValue(fetched1);

    // step 5: Validate - compute a small validation score using results of the operations
    validation_score = (int)(root != (cJSON *)0) + (int)(nums != (cJSON *)0) + (int)(greeting != (cJSON *)0) + (int)(add_ok_string) + (int)(add_ok_array) + (int)(array_size == 3) + (int)(val0 == 1.0) + (int)(val1 == 2.0);
    (void)validation_score;

    // step 6: Cleanup - free all allocated cJSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
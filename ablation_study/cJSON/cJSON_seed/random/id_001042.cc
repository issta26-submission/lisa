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
//<ID> 1042
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *unused_json = "{\"placeholder\":true}";
    cJSON *root = (cJSON *)0;
    cJSON *nums = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    cJSON *name_str = (cJSON *)0;
    int array_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    int validation_score = 0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;

    // step 2: Setup - create an object, an array and items to populate them
    root = cJSON_CreateObject();
    nums = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.5);
    name_str = cJSON_CreateString("example_name");

    // step 3: Operate - build structure: add numbers to array and add array and name to root using CS variant
    add_ok1 = cJSON_AddItemToArray(nums, n0);
    add_ok2 = cJSON_AddItemToArray(nums, n1);
    add_ok3 = cJSON_AddItemToObjectCS(root, "nums", nums);
    cJSON_AddItemToObjectCS(root, "name", name_str);

    // step 4: Operate continued - fetch array size and numeric values
    array_size = cJSON_GetArraySize(nums);
    first_item = cJSON_GetArrayItem(nums, 0);
    second_item = cJSON_GetArrayItem(nums, 1);
    val0 = cJSON_GetNumberValue(first_item);
    val1 = cJSON_GetNumberValue(second_item);

    // step 5: Validate - derive a small validation score exercising the used APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(nums != (cJSON *)0) + (int)(add_ok1) + (int)(add_ok2) + (int)(add_ok3) + (int)(array_size == 2) + (int)(val0 == 10.0) + (int)(val1 == 20.5);
    (void)validation_score;
    (void)unused_json;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 999
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "numbers";
    const char *ans_key = "answer";
    const char *flag_key = "is_true";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    cJSON_bool israw_n1 = (cJSON_bool)0;
    double first_val = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array to hold numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - attach array to root and populate it with number items; also add a number as an object member
    cJSON_AddItemToObject(root, array_key, arr);
    n1 = cJSON_CreateNumber(3.14);
    add_ok1 = cJSON_AddItemToArray(arr, n1);
    n2 = cJSON_CreateNumber(2.71);
    add_ok2 = cJSON_AddItemToArray(arr, n2);
    n3 = cJSON_CreateNumber(42.0);
    add_ok3 = cJSON_AddItemToObject(root, ans_key, n3);

    // step 4: Operate - add a boolean to the root using AddBoolToObject and inspect an item with IsRaw
    bool_item = cJSON_AddBoolToObject(root, flag_key, (cJSON_bool)1);
    israw_n1 = cJSON_IsRaw(n1);

    // step 5: Validate - retrieve the first array element numeric value and compute a small validation score
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)add_ok3 + (int)(bool_item != (cJSON *)0) + (int)(israw_n1 == 0) + (int)(first_val == 3.14);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
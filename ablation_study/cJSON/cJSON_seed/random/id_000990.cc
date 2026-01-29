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
//<ID> 990
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *single_key = "single";
    const char *bool_key = "enabled";
    double v1 = 3.1415;
    double v2 = 2.7182;
    double v3 = 42.0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *added_bool = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    cJSON_bool israw_n1 = (cJSON_bool)0;
    cJSON_bool israw_arr = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(v1);
    n2 = cJSON_CreateNumber(v2);
    n3 = cJSON_CreateNumber(v3);

    // step 3: Configure - add numbers to the array and attach array and single number to the root; add a boolean field
    add_ok1 = cJSON_AddItemToArray(arr, n1);
    add_ok2 = cJSON_AddItemToArray(arr, n2);
    add_ok3 = cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, arr_key, arr);
    cJSON_AddItemToObject(root, single_key, cJSON_CreateNumber(7.0));
    added_bool = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);

    // step 4: Operate - inspect some items for their raw status
    israw_n1 = cJSON_IsRaw(n1);
    israw_arr = cJSON_IsRaw(arr);

    // step 5: Validate - compute a simple validation score exercising returns and states
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)add_ok3 + (int)(added_bool != (cJSON *)0) + (int)israw_n1 + (int)(!israw_arr);
    (void)validation_score;

    // step 6: Cleanup - delete root (frees attached items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
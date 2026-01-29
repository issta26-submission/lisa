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
//<ID> 1009
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "items";
    const char *obj_str_key = "title";
    const char *flag_key = "enabled";
    const char *text0 = "alpha";
    const char *text1 = "beta";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *sitem0 = (cJSON *)0;
    cJSON *sitem1 = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    const char *val0 = (const char *)0;
    const char *val1 = (const char *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach the array to the root using AddItemToObject
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_ok = cJSON_AddItemToObject(root, array_key, arr);

    // step 3: Configure - create string items and a true boolean, add strings into the array and add boolean and an extra string to the root object
    sitem0 = cJSON_CreateString(text0);
    sitem1 = cJSON_CreateString(text1);
    add_ok = cJSON_AddItemToArray(arr, sitem0);
    add_ok = cJSON_AddItemToArray(arr, sitem1);
    bool_item = cJSON_CreateTrue();
    add_ok = cJSON_AddItemToObject(root, flag_key, bool_item);
    add_ok = cJSON_AddItemToObject(root, obj_str_key, cJSON_CreateString("container"));

    // step 4: Operate - retrieve array elements using cJSON_GetArrayItem and obtain their string values
    fetched0 = cJSON_GetArrayItem(arr, 0);
    fetched1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetStringValue(fetched0);
    val1 = cJSON_GetStringValue(fetched1);

    // step 5: Validate - compute a small validation score exercising pointers and content
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(sitem0 != (cJSON *)0) + (int)(sitem1 != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(add_ok) + (int)(val0 != (const char *)0 && val0[0] == 'a') + (int)(val1 != (const char *)0 && val1[0] == 'b');
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
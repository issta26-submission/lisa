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
//<ID> 1051
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *root = (cJSON *)0;
    cJSON *str_array = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    char *second_value = (char *)0;
    char *printed = (char *)0;
    cJSON_bool add_result = (cJSON_bool)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Setup - create a root object and a cJSON string array
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray((const char *const *)strings, 3);

    // step 3: Configure - add the string array into the root object under the key "strings"
    add_result = cJSON_AddItemToObject(root, "strings", str_array);

    // step 4: Operate - retrieve the second element from the array and obtain its string value; produce and free a printout
    second_item = cJSON_GetArrayItem(str_array, 1);
    second_value = cJSON_GetStringValue(second_item);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score using the results
    arr_size = cJSON_GetArraySize(str_array);
    validation_score = (int)(root != (cJSON *)0) + (int)(str_array != (cJSON *)0) + (int)(second_item != (cJSON *)0) + (int)(second_value != (char *)0) + (int)(arr_size == 3) + (int)(add_result);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
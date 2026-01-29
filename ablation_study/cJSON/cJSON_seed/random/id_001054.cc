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
//<ID> 1054
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *const strings[] = { "alpha", "beta", "gamma" };
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    char *second_value = (char *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool add_ok = (cJSON_bool)0;

    // step 2: Setup - create a root object and a string array
    root = cJSON_CreateObject();
    arr = cJSON_CreateStringArray(strings, 3);

    // step 3: Configure - attach the array to the root object
    add_ok = cJSON_AddItemToObject(root, "letters", arr);

    // step 4: Operate - fetch the second element from the array and produce a print
    second_item = cJSON_GetArrayItem(arr, 1);
    second_value = cJSON_GetStringValue(second_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - compute a small validation score from results
    arr_size = cJSON_GetArraySize(arr);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(second_item != (cJSON *)0) + (int)(second_value != (char *)0) + (int)(arr_size == 3) + (int)(add_ok);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete the cJSON structure which releases attached items
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
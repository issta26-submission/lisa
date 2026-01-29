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
//<ID> 1050
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *colors[] = { "red", "green", "blue" };
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    const char *val0 = (const char *)0;
    const char *val1 = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Setup - create a root object and a string array
    root = cJSON_CreateObject();
    arr = cJSON_CreateStringArray(colors, 3);

    // step 3: Configure - add the array to the root object
    add_ok = cJSON_AddItemToObject(root, "colors", arr);

    // step 4: Operate - fetch array items, read their string values, print and free the printed representation
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetStringValue(item0);
    val1 = cJSON_GetStringValue(item1);
    arr_size = cJSON_GetArraySize(arr);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score based on results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(add_ok) + (int)(arr_size == 3) + (int)(val0 != (const char *)0) + (int)(val1 != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
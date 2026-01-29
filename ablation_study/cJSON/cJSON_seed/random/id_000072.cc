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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_arr = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    const char *first_val = (const char *)0;

    // step 2: Setup - create root and a string array
    const char *colors[] = { "red", "green", "blue" };
    root = cJSON_CreateObject();
    str_arr = cJSON_CreateStringArray(colors, 3);

    // step 3: Configure - attach array, add number and a true boolean, and add a reference to the array
    cJSON_AddItemToObject(root, "colors", str_arr);
    num_item = cJSON_AddNumberToObject(root, "count", 3.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);
    cJSON_AddItemReferenceToObject(root, "colors_ref", str_arr);

    // step 4: Operate - read first color and store as a separate named string in root
    first_item = cJSON_GetArrayItem(str_arr, 0);
    first_val = cJSON_GetStringValue(first_item);
    cJSON_AddItemToObject(root, "first_color", cJSON_CreateString(first_val));

    // step 5: Validate - add a validated boolean based on the true item
    cJSON_AddBoolToObject(root, "validated", cJSON_IsTrue(true_item));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
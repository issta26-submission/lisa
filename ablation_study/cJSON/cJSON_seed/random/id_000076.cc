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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    const char *colors[] = { "red", "green", "blue" };
    cJSON *str_arr = cJSON_CreateStringArray(colors, 3);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    const char *first_val = (const char *)0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize / Configure - add items to root
    cJSON_AddItemToObject(root, "colors", str_arr);
    num_item = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddItemToObject(root, "enabled", true_item);
    add_ok = cJSON_AddItemReferenceToObject(root, "colors_ref", str_arr);

    // step 3: Operate - retrieve first color from the array and reuse it
    retrieved_arr = cJSON_GetObjectItem(root, "colors");
    first_item = cJSON_GetArrayItem(retrieved_arr, 0);
    first_val = cJSON_GetStringValue(first_item);

    // step 4: Validate - store the retrieved first color back into root
    cJSON_AddItemToObject(root, "first_color", cJSON_CreateString(first_val));

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
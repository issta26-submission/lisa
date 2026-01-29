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
//<ID> 1058
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *const strings[] = {"alpha", "beta", "gamma"};
    cJSON *root = (cJSON *)0;
    cJSON *str_array = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    char *second_value = (char *)0;
    char *printed = (char *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray(strings, 3);

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "items", str_array);

    // step 4: Operate
    second_elem = cJSON_GetArrayItem(str_array, 1);
    second_value = cJSON_GetStringValue(second_elem);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(str_array != (cJSON *)0) + (int)(second_elem != (cJSON *)0) + (int)(second_value != (char *)0) + (int)add_ok;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
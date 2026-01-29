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
//<ID> 1053
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
    cJSON *second_item = (cJSON *)0;
    char *second_value = (char *)0;
    int array_size = 0;
    cJSON_bool added = (cJSON_bool)0;
    char *tmp_buf = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a string array
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray(strings, 3);

    // step 3: Configure - attach the string array to the root object
    added = cJSON_AddItemToObject(root, "letters", str_array);

    // step 4: Operate - fetch the second element and measure array size, allocate a small buffer
    second_item = cJSON_GetArrayItem(str_array, 1);
    second_value = cJSON_GetStringValue(second_item);
    array_size = cJSON_GetArraySize(str_array);
    tmp_buf = (char *)cJSON_malloc(32);
    memset(tmp_buf, 0, 32);

    // step 5: Validate - compute a small validation score using results
    validation_score = (int)(root != (cJSON *)0) + (int)(str_array != (cJSON *)0) + (int)(added) + (int)(second_item != (cJSON *)0) + (int)(second_value != (char *)0) + (int)(array_size == 3);
    (void)validation_score;

    // step 6: Cleanup - free allocated buffer and delete root (which frees attached items)
    cJSON_free((void *)tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
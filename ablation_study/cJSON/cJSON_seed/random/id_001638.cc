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
//<ID> 1638
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool first_is_string = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    num_item = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    first = cJSON_GetArrayItem(arr, 0);
    first_is_string = cJSON_IsString(first);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)(arr != (cJSON *)0)
        + (int)(s1 != (cJSON *)0)
        + (int)(s2 != (cJSON *)0)
        + (int)first_is_string
        + (int)(arr_size == 2)
        + (int)(num_item != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)arr_size;
    (void)first;
    (void)num_item;
    (void)first_is_string;
    // API sequence test completed successfully
    return 66;
}
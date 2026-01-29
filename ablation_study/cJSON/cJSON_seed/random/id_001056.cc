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
//<ID> 1056
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *strings[] = {"apple", "banana", "cherry"};
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *second_value = (const char *)0;
    int arr_size = 0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateStringArray((const char *const *)strings, 3);

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "fruits", arr);

    // step 4: Operate
    fetched_arr = cJSON_GetObjectItem(root, "fruits");
    second_item = cJSON_GetArrayItem(fetched_arr, 1);
    second_value = cJSON_GetStringValue(second_item);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(fetched_arr);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(add_ok) + (int)(arr_size == 3) + (int)(second_value != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
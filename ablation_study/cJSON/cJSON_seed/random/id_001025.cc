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
//<ID> 1025
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"numbers\": [10, 20], \"meta\": {\"count\": 2}}";
    const char *numbers_key = "numbers";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *new_num = (cJSON *)0;
    cJSON *fetched2 = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double vnew = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse JSON and obtain the array
    root = cJSON_Parse(json_text);
    arr = cJSON_GetObjectItem(root, numbers_key);

    // step 3: Configure - create a new number and add it to the parsed array
    new_num = cJSON_CreateNumber(30.5);
    add_ok = cJSON_AddItemToArray(arr, new_num);

    // step 4: Operate - fetch existing and newly added items and read their numeric values
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    fetched2 = cJSON_GetArrayItem(arr, 2);
    v0 = cJSON_GetNumberValue(item0);
    v1 = cJSON_GetNumberValue(item1);
    vnew = cJSON_GetNumberValue(fetched2);

    // step 5: Validate - exercise type check and value checks into a small score
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)cJSON_IsArray(arr) + (int)(item0 != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(new_num != (cJSON *)0) + (int)(add_ok) + (int)(v0 == 10.0) + (int)(v1 == 20.0) + (int)(vnew > 30.0 && vnew < 31.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
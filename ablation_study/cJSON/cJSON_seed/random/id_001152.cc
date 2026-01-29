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
//<ID> 1152
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
    cJSON *str_item0 = (cJSON *)0;
    cJSON *str_item1 = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    cJSON_bool is_elem0_string = 0;
    cJSON_bool is_elem1_string = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array container
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create two string items and add them to the array
    str_item0 = cJSON_CreateString("alpha");
    str_item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, str_item0);
    cJSON_AddItemToArray(arr, str_item1);

    // step 4: Operate - add a raw JSON fragment into root and fetch items back
    raw_item = cJSON_AddRawToObject(root, "raw_fragment", "{\"embedded\":true,\"count\":2}");
    fetched_arr = cJSON_GetObjectItem(root, "items");
    fetched_elem0 = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_elem1 = cJSON_GetArrayItem(fetched_arr, 1);
    is_elem0_string = cJSON_IsString(fetched_elem0);
    is_elem1_string = cJSON_IsString(fetched_elem1);

    // step 5: Validate - compute a simple validation score from retrieved data
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_elem0 != (cJSON *)0) + (int)(fetched_elem1 != (cJSON *)0) + (int)is_elem0_string + (int)is_elem1_string + (int)(raw_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items including the raw fragment and array elements
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
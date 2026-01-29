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
//<ID> 1158
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
    cJSON *raw_meta = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_elem = (cJSON *)0;
    char *fetched_string = (char *)0;
    cJSON_bool is_string_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create string items and append them to the array
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 4: Operate - add a raw JSON member to root and fetch array element
    raw_meta = cJSON_AddRawToObject(root, "meta", "{\"enabled\":true,\"count\":2}");
    fetched_arr = cJSON_GetObjectItem(root, "items");
    fetched_elem = cJSON_GetArrayItem(fetched_arr, 1);
    fetched_string = cJSON_GetStringValue(fetched_elem);
    is_string_flag = cJSON_IsString(fetched_elem);

    // step 5: Validate - compute a simple validation score from returned pointers and flags
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_elem != (cJSON *)0) + (int)(fetched_string != (char *)0) + (int)(is_string_flag != 0) + (int)(raw_meta != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
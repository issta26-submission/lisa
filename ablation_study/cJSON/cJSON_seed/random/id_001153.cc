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
//<ID> 1153
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
    cJSON *fetched_elem1 = (cJSON *)0;
    cJSON *raw_added = (cJSON *)0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array to hold strings
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create string items and add them to the array
    str_item0 = cJSON_CreateString("alpha");
    str_item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, str_item0);
    cJSON_AddItemToArray(arr, str_item1);

    // step 4: Operate - fetch an array element and add a raw JSON fragment to the root
    fetched_elem1 = cJSON_GetArrayItem(arr, 1);
    raw_added = cJSON_AddRawToObject(root, "metadata", "{\"count\":2,\"valid\":true}");

    // step 5: Validate - check string types, serialize for round-trip, and compute a simple score
    validation_score = (int)(root != (cJSON *)0) + (int)cJSON_IsString(str_item0) + (int)cJSON_IsString(fetched_elem1) + (int)(raw_added != (cJSON *)0);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
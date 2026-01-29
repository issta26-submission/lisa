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
//<ID> 1156
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
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    cJSON *raw_added = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");

    // step 3: Configure - add strings to array and attach array to root, add raw JSON to root
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "letters", arr);
    raw_added = cJSON_AddRawToObject(root, "metadata", "{\"generated\":true,\"version\":1}");

    // step 4: Operate - retrieve array and element, verify types
    fetched_arr = cJSON_GetObjectItem(root, "letters");
    fetched_elem1 = cJSON_GetArrayItem(fetched_arr, 1);
    validation_score = (int)cJSON_IsString(fetched_elem1) + (int)cJSON_IsString(s1) + (int)(raw_added != (cJSON *)0);

    // step 5: Validate - serialize and free buffer
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 6: Cleanup - delete root which frees contained items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}
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
//<ID> 1155
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *raw_meta = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create string items and add them to the array; add a raw JSON member to root
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    raw_meta = cJSON_AddRawToObject(root, "meta", "{\"version\":1}");

    // step 4: Operate - fetch first array element and perform type checks
    fetched_elem0 = cJSON_GetArrayItem(arr, 0);
    validation_score = (int)cJSON_IsString(str1) + (int)cJSON_IsString(str2) + (int)cJSON_IsString(fetched_elem0) + (int)cJSON_IsString(raw_meta);

    // step 5: Validate - serialize and free the serialization, include validation_score in a void cast to use it
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached array and items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
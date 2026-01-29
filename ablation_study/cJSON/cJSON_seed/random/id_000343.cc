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
//<ID> 343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *outer = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *ref = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item_via_ref = (cJSON *)0;
    cJSON *second_via_ref = (cJSON *)0;
    char *json1 = (char *)0;
    char *json2 = (char *)0;
    char *json3 = (char *)0;
    cJSON_bool res = (cJSON_bool)0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    outer = cJSON_CreateArray();
    str1 = cJSON_CreateString("hello");
    str2 = cJSON_CreateString("world");

    // step 3: Configure
    res = cJSON_AddItemToArray(arr, str1);
    res = cJSON_AddItemToArray(arr, str2);
    ref = cJSON_CreateArrayReference(arr);
    res = cJSON_AddItemToArray(outer, ref);

    // step 4: Operate
    item0 = cJSON_GetArrayItem(arr, 0);
    json1 = cJSON_PrintUnformatted(item0);
    item_via_ref = cJSON_GetArrayItem(outer, 0);
    json2 = cJSON_PrintUnformatted(item_via_ref);
    second_via_ref = cJSON_GetArrayItem(item_via_ref, 1);
    json3 = cJSON_PrintUnformatted(second_via_ref);

    // step 5: Validate
    (void)res;
    (void)item0;
    (void)item_via_ref;
    (void)second_via_ref;
    cJSON_free(json1);
    cJSON_free(json2);
    cJSON_free(json3);

    // step 6: Cleanup
    cJSON_Delete(outer);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 683
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
    cJSON *arr2 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    char *printed = (char *)0;
    char *minify_target = (char *)0;
    cJSON_bool added_manual = 0;
    cJSON_bool added_to_arr = 0;
    int size_manual = 0;
    int size_constructed = 0;
    int combined_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    n1 = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_to_arr = cJSON_AddItemToArray(arr, s1);
    added_to_arr = cJSON_AddItemToArray(arr, s2);
    added_to_arr = cJSON_AddItemToArray(arr, n1);
    added_manual = cJSON_AddItemToObject(root, "manual_array", arr);
    arr2 = cJSON_AddArrayToObject(root, "constructed_array");
    cJSON_AddItemToArray(arr2, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr2, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr2, cJSON_CreateNumber(3.0));

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    minify_target = printed;
    cJSON_Minify(minify_target);
    size_manual = cJSON_GetArraySize(cJSON_GetObjectItem(root, "manual_array"));
    size_constructed = cJSON_GetArraySize(cJSON_GetObjectItem(root, "constructed_array"));
    combined_size = size_manual + size_constructed;

    // step 5: Validate
    (void)added_manual;
    (void)added_to_arr;
    (void)size_manual;
    (void)size_constructed;
    (void)combined_size;
    (void)minify_target;
    (void)printed;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 680
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr1 = (cJSON *)0;
    cJSON *arr2 = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    char *printed = (char *)0;
    int size1 = 0;
    int size2 = 0;
    int total_size = 0;
    cJSON_bool added_arr1 = 0;
    cJSON_bool added_s0 = 0;
    cJSON_bool added_s1 = 0;
    cJSON_bool added_n0 = 0;
    cJSON_bool added_n1 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr1 = cJSON_CreateArray();
    s0 = cJSON_CreateString("alpha");
    s1 = cJSON_CreateString("beta");
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.0);

    // step 3: Configure
    added_s0 = cJSON_AddItemToArray(arr1, s0);
    added_s1 = cJSON_AddItemToArray(arr1, s1);
    added_arr1 = cJSON_AddItemToObject(root, "first", arr1);
    arr2 = cJSON_AddArrayToObject(root, "second");
    added_n0 = cJSON_AddItemToArray(arr2, n0);
    added_n1 = cJSON_AddItemToArray(arr2, n1);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    size1 = cJSON_GetArraySize(arr1);
    size2 = cJSON_GetArraySize(arr2);
    total_size = size1 + size2;

    // step 5: Validate
    (void)added_s0;
    (void)added_s1;
    (void)added_arr1;
    (void)added_n0;
    (void)added_n1;
    (void)size1;
    (void)size2;
    (void)total_size;
    (void)printed;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}
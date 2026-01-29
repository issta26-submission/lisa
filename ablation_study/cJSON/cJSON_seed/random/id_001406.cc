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
//<ID> 1406
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *false_node = (cJSON *)0;
    cJSON *ref_to_n1 = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON *flag_node = (cJSON *)0;
    int arr_size = 0;
    int is_flag_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root, an array and elements including a false boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateString("first");
    n2 = cJSON_CreateString("second");
    false_node = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach members to root using CS variant for keys
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObjectCS(root, "data", arr);
    cJSON_AddItemToObjectCS(root, "flag", false_node);

    // step 4: Operate - create an object reference to the first element and retrieve the first array item
    ref_to_n1 = cJSON_CreateObjectReference(n1);
    cJSON_AddItemToObjectCS(root, "ref_first", ref_to_n1);
    got_item = cJSON_GetArrayItem(arr, 0);

    // step 5: Validate - compute simple validation metrics from the structure
    arr_size = cJSON_GetArraySize(arr);
    flag_node = cJSON_GetObjectItem(root, "flag");
    is_flag_false = (int)cJSON_IsFalse(flag_node);
    validation_score = arr_size + is_flag_false + (int)(got_item == n1) + (int)(ref_to_n1 != (cJSON *)0);

    // step 6: Cleanup - free all cJSON resources
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)is_flag_false;
    (void)got_item;
    return 66;
}
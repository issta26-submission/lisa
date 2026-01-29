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
//<ID> 931
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "array";
    const char *str_key = "label";
    const char *label_val = "ref_label";
    char minjson[] = " { \"a\" : 1, \"b\" : [ 2, 3 ] } ";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    int validation_score = 0;
    int arr_size = 0;
    const char *extracted_str = (const char *)0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    child = cJSON_CreateString("child_value");

    // step 3: Configure
    arr_ref = cJSON_CreateArrayReference(child);
    cJSON_AddItemToArray(arr, arr_ref);
    cJSON_AddItemToObject(root, arr_key, arr);

    // step 4: Operate
    str_item = cJSON_AddStringToObject(root, str_key, label_val);
    cJSON_Minify(minjson);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    extracted_str = cJSON_GetStringValue(str_item);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(child != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(arr_size > 0) + (int)(extracted_str != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}
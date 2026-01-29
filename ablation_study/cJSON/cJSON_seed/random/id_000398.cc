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
//<ID> 398
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
    cJSON *str_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    cJSON_bool add_ref_result = 0;
    int arr_size = 0;
    char *extracted = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example-string");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    flag_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    add_ref_result = cJSON_AddItemReferenceToObject(root, "alias", str_item);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    ref_item = cJSON_GetObjectItem(root, "alias");
    extracted = cJSON_GetStringValue(ref_item);
    (void)add_ref_result;
    (void)arr_size;
    (void)extracted;
    (void)flag_item;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 404
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
    cJSON *num_item = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    char *printed = (char *)0;
    const char *extracted_str = (const char *)0;
    double extracted_num = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("value0");
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddBoolToObject(root, "active", 1);

    // step 4: Operate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "active");

    // step 5: Validate
    extracted_str = cJSON_GetStringValue(got0);
    extracted_num = cJSON_GetNumberValue(got1);
    (void)extracted_str;
    (void)extracted_num;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
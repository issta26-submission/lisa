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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *meta = NULL;
    cJSON *label = NULL;
    char *printed = NULL;
    cJSON_bool add_res1 = 0;
    cJSON_bool add_res2 = 0;
    double set_res = 0.0;

    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_res1 = cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    num1 = cJSON_CreateNumber(1.0);
    add_res2 = cJSON_AddItemToArray(arr, num1);
    set_res = cJSON_SetNumberHelper(num1, 42.0);
    num2 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(arr, num2);
    meta = cJSON_CreateObject();
    label = cJSON_CreateString("metadata-label");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON_AddItemReferenceToArray(arr, meta);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
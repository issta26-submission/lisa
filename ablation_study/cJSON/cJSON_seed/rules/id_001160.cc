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
//<ID> 1160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *bool_true = NULL;
    cJSON *bool_false = NULL;
    cJSON *tmp_item = NULL;
    char *printed = NULL;
    double first_value = 0.0;
    int size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    bool_true = cJSON_CreateTrue();
    bool_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "enabled", bool_true);
    cJSON_AddItemToObject(root, "disabled", bool_false);

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(arr);
    tmp_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(tmp_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
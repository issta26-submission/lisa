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
//<ID> 1161
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
    cJSON *bool_t = NULL;
    cJSON *bool_f = NULL;
    cJSON *name = NULL;
    cJSON *computed = NULL;
    char *printed = NULL;
    char *dyn = NULL;
    double value0 = 0.0;
    int size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(42.5);
    num1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    bool_t = cJSON_CreateTrue();
    bool_f = cJSON_CreateFalse();
    dyn = (char *)cJSON_malloc(8);
    memset(dyn, 0, 8);
    dyn[0] = 't';
    dyn[1] = 'e';
    dyn[2] = 's';
    dyn[3] = 't';
    dyn[4] = '\0';
    name = cJSON_CreateString(dyn);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "enabled", bool_t);
    cJSON_AddItemToObject(root, "disabled", bool_f);
    cJSON_AddItemToObject(root, "label", name);

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(arr);
    value0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    computed = cJSON_CreateNumber(value0 * (double)size);
    cJSON_AddItemToObject(root, "computed", computed);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(dyn);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
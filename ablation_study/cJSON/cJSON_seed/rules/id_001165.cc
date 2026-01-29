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
//<ID> 1165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *num_item = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *summary = NULL;
    double first_value = 0.0;
    int array_size = 0;
    char dyn[16];
    char *printed = NULL;

    // step 2: Setup / Configure
    memset(dyn, 0, sizeof(dyn));
    dyn[0] = 't'; dyn[1] = 'a'; dyn[2] = 'g'; dyn[3] = '\0';
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.5);
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToArray(array, false_item);
    cJSON_AddItemToObject(root, "entries", array);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString(dyn));

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(array);
    first_value = cJSON_GetNumberValue(num_item);
    summary = cJSON_CreateObject();
    cJSON_AddItemToObject(summary, "count", cJSON_CreateNumber((double)array_size));
    cJSON_AddItemToObject(summary, "first", cJSON_CreateNumber(first_value));
    cJSON_AddItemToObject(root, "summary", summary);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
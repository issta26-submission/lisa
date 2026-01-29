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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *initial_item = NULL;
    cJSON *double_item = NULL;
    double initial_value = 0.0;
    double computed_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, str_item);
    initial_item = cJSON_AddNumberToObject(root, "initial", 3.14);

    // step 3: Operate / Validate
    initial_value = cJSON_GetNumberValue(initial_item);
    computed_value = initial_value * 2.0;
    double_item = cJSON_AddNumberToObject(root, "double", computed_value);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
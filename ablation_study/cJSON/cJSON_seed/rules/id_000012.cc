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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *str_item = NULL;
    cJSON *num_item = NULL;
    cJSON *double_item = NULL;
    double value = 0.0;
    double doubled = 0.0;

    // step 2: Setup (Initialize and configure JSON structure)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "letters", arr);
    num_item = cJSON_AddNumberToObject(root, "count", 21.0);

    // step 3: Operate / Validate (read number, compute, and store result)
    value = cJSON_GetNumberValue(num_item);
    doubled = value * 2.0;
    double_item = cJSON_AddNumberToObject(root, "double_count", doubled);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
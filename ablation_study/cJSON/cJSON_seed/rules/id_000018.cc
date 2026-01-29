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
//<ID> 18
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
    cJSON *msg = cJSON_CreateString("example_message");
    cJSON *pi_item = NULL;
    cJSON *arr_first = NULL;
    char *printed = NULL;
    double pi_value = 0.0;
    double first_value = 0.0;
    int validation = 0;

    // step 2: Setup (initialize and configure JSON structure)
    cJSON_AddItemToObject(root, "message", msg);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));

    // step 3: Operate / Validate (retrieve numbers and compute validation)
    pi_item = cJSON_GetObjectItem(root, "pi");
    pi_value = cJSON_GetNumberValue(pi_item);
    arr_first = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(arr_first);
    validation = (int)(pi_value + first_value);
    cJSON_AddNumberToObject(root, "validation", (double)validation);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}
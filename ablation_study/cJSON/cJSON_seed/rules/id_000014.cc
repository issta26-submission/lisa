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
//<ID> 14
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
    cJSON *num_item2 = NULL;
    char *printed = NULL;
    double extracted = 0.0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str_item = cJSON_CreateString("example_string");
    cJSON_AddItemToArray(arr, str_item);
    num_item = cJSON_AddNumberToObject(root, "base_value", 3.1415);

    // step 3: Operate/Validate
    extracted = cJSON_GetNumberValue(num_item);
    num_item2 = cJSON_AddNumberToObject(root, "derived_value", extracted * 2.0);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 13
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
    cJSON *str = cJSON_CreateString("example_string");
    cJSON *orig_num_item = NULL;
    cJSON *copy_num_item = NULL;
    double extracted_value = 0.0;

    // step 2: Setup
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "items", arr);
    orig_num_item = cJSON_AddNumberToObject(root, "original", 42.0);

    // step 3: Operate/Validate
    extracted_value = cJSON_GetNumberValue(orig_num_item);
    copy_num_item = cJSON_AddNumberToObject(root, "copy", extracted_value);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 11
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
    cJSON *str_root = NULL;
    cJSON *str_arr = NULL;
    cJSON *num_item = NULL;
    const cJSON *retrieved_num = NULL;
    double value = 0.0;
    char *out = NULL;

    // step 2: Setup
    str_root = cJSON_CreateString("root_string");
    cJSON_AddItemToObject(root, "name", str_root);
    str_arr = cJSON_CreateString("array_string");
    cJSON_AddItemToArray(arr, str_arr);
    cJSON_AddItemToObject(root, "items", arr);
    num_item = cJSON_AddNumberToObject(root, "value", 3.1415);

    // step 3: Operate/Validate
    retrieved_num = cJSON_GetObjectItem(root, "value");
    value = cJSON_GetNumberValue(retrieved_num);
    cJSON_AddNumberToObject(root, "value_times_two", value * 2.0);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
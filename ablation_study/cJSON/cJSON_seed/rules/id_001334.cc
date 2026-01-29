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
//<ID> 1334
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *num_plus_item = NULL;
    double num_val = 0.0;
    double num_plus_val = 0.0;
    const char *orig_str = "example_string";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "num", num_item);
    str_item = cJSON_CreateString(orig_str);
    cJSON_AddItemToObject(root, "str", str_item);

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(root, "num");
    num_val = cJSON_GetNumberValue(num_item);
    num_plus_item = cJSON_CreateNumber(num_val + 10.0);
    cJSON_AddItemToObject(root, "num_plus", num_plus_item);
    num_plus_item = cJSON_GetObjectItem(root, "num_plus");
    num_plus_val = cJSON_GetNumberValue(num_plus_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
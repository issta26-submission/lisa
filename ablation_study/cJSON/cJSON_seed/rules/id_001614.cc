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
//<ID> 1614
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *raw_item = NULL;
    cJSON *null_item = NULL;
    cJSON *num_item = NULL;
    cJSON *lookup_item = NULL;
    double extracted_value = 0.0;
    cJSON_bool null_check = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    num_item = cJSON_AddNumberToObject(root, "pi", 3.1415);
    raw_item = cJSON_CreateRaw("{\"rawkey\":123}");
    cJSON_AddItemToObject(child, "payload", raw_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", null_item);

    // step 3: Operate / Validate
    lookup_item = cJSON_GetObjectItem(root, "pi");
    extracted_value = cJSON_GetNumberValue(lookup_item);
    null_check = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
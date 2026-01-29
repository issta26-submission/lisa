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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig_item = cJSON_CreateString("original");
    cJSON *num_item = cJSON_CreateNumber(2.718);
    cJSON *replacement = cJSON_CreateString("replaced");
    const char raw_json[] = "{\"raw_key\":true}";
    cJSON *derived_num = NULL;
    cJSON *retrieved_num = NULL;
    double retrieved_value = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", orig_item);
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddRawToObject(root, "raw", raw_json);

    // step 3: Operate / Validate
    retrieved_num = cJSON_GetObjectItem(root, "num");
    retrieved_value = cJSON_GetNumberValue(retrieved_num);
    derived_num = cJSON_CreateNumber(retrieved_value + 1.0);
    cJSON_AddItemToObject(root, "num_plus_one", derived_num);
    replaced = cJSON_ReplaceItemViaPointer(root, orig_item, replacement);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
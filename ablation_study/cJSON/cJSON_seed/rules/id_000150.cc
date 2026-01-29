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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    cJSON *name_item = cJSON_CreateString("pi");
    cJSON *replacement_num = NULL;
    cJSON *raw_item = NULL;
    cJSON *retrieved_num_item = NULL;
    char *printed = NULL;
    double value = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "name", name_item);
    raw_item = cJSON_AddRawToObject(root, "meta", "{\"raw_key\":true}");

    // step 3: Operate / Validate
    retrieved_num_item = cJSON_GetObjectItem(root, "number");
    value = cJSON_GetNumberValue(retrieved_num_item);
    replacement_num = cJSON_CreateNumber(value * 2.0);
    replaced = cJSON_ReplaceItemViaPointer(root, name_item, replacement_num);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
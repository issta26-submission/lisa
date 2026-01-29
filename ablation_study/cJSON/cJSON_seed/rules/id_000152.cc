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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count_item = cJSON_CreateNumber(2021.0);
    cJSON *replacement = cJSON_CreateNumber(2022.0);
    cJSON *raw_item = NULL;
    cJSON *label_item = cJSON_CreateString("example");
    cJSON *new_count_item = NULL;
    char *printed = NULL;
    double original_value = 0.0;
    double new_value = 0.0;
    double combined = 0.0;
    cJSON_bool add_ok = 0;
    cJSON_bool replace_ok = 0;

    // step 2: Setup / Configure
    add_ok = cJSON_AddItemToObject(root, "count", count_item);
    raw_item = cJSON_AddRawToObject(root, "raw", "{\"inner\":true}");
    cJSON_AddItemToObject(root, "label", label_item);

    // step 3: Operate / Validate
    original_value = cJSON_GetNumberValue(count_item);
    replace_ok = cJSON_ReplaceItemViaPointer(root, count_item, replacement);
    new_count_item = cJSON_GetObjectItem(root, "count");
    new_value = cJSON_GetNumberValue(new_count_item);
    combined = original_value + new_value;
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)add_ok;
    (void)replace_ok;
    (void)raw_item;
    (void)combined;
    return 66;
}
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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON *number_item = NULL;
    cJSON *new_item = NULL;
    cJSON *raw_item = NULL;
    char *printed = NULL;
    double original_val = 0.0;
    double new_val = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num);
    raw_item = cJSON_AddRawToObject(root, "raw", "{\"inner\":true}");

    // step 3: Operate / Validate
    number_item = cJSON_GetObjectItem(root, "number");
    original_val = cJSON_GetNumberValue(number_item);
    new_item = cJSON_CreateNumber(original_val + 10.0);
    replaced = cJSON_ReplaceItemViaPointer(root, number_item, new_item);
    printed = cJSON_PrintUnformatted(root);
    number_item = cJSON_GetObjectItem(root, "number");
    new_val = cJSON_GetNumberValue(number_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)raw_item;
    (void)replaced;
    (void)new_val;
    return 66;
}
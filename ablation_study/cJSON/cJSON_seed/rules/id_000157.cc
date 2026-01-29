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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *name_item = cJSON_CreateString("original");
    cJSON *replacement = cJSON_CreateString("replaced");
    double num_value = 0.0;
    cJSON_bool replaced = 0;
    char *out = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "count", num_item);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddRawToObject(root, "raw", "{\"nested\":{\"v\":1}}");

    // step 3: Operate / Validate
    num_value = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "count_copy", num_value + 1.0);
    replaced = cJSON_ReplaceItemViaPointer(root, name_item, replacement);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1593
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
    cJSON *null_item = NULL;
    cJSON *fetched = NULL;
    char *out = NULL;
    double orig_value = 123.456;
    double read_value = 0.0;
    cJSON_bool was_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(orig_value);
    cJSON_AddItemToObject(root, "number", num_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_false", null_item);

    // step 3: Operate / Validate
    fetched = cJSON_GetObjectItem(root, "number");
    read_value = cJSON_GetNumberValue(fetched);
    was_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "maybe_false"));
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "maybe_false");
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
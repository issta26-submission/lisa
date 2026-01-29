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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"a\": 1.5, \"b\": {\"c\": 2.25}}";
    size_t len = strlen(json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    cJSON *dup = NULL;
    cJSON *item_a = NULL;
    cJSON *child_b = NULL;
    cJSON *item_c = NULL;
    double val_a = 0.0;
    double val_c = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    item_a = cJSON_GetObjectItem(root, "a");
    child_b = cJSON_GetObjectItem(root, "b");
    item_c = cJSON_GetObjectItem(child_b, "c");

    // step 3: Operate / Validate
    val_a = cJSON_GetNumberValue(item_a);
    val_c = cJSON_GetNumberValue(item_c);
    dup = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup, 1);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
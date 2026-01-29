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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"a\":1,\"b\":{\"c\":2}}";
    size_t len = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, len, &parse_end, 0);
    cJSON *a_item = cJSON_GetObjectItem(root, "a");
    cJSON *b_item = cJSON_GetObjectItem(root, "b");
    double a_value = cJSON_GetNumberValue(a_item);
    cJSON *c_item = cJSON_GetObjectItem(b_item, "c");
    double c_value = cJSON_GetNumberValue(c_item);

    // step 2: Setup / Configure equivalent structure
    cJSON *manual = cJSON_CreateObject();
    cJSON *b_manual = cJSON_CreateObject();
    cJSON_AddItemToObject(b_manual, "c", cJSON_CreateNumber(c_value));
    cJSON_AddItemToObject(manual, "a", cJSON_CreateNumber(a_value));
    cJSON_AddItemToObject(manual, "b", b_manual);

    // step 3: Operate / Validate
    cJSON_bool equal = cJSON_Compare(root, manual, 1);
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(manual);

    // API sequence test completed successfully
    return 66;
}
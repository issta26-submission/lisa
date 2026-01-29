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
//<ID> 1533
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *child = NULL;
    cJSON *dup = NULL;
    cJSON *num = NULL;
    cJSON *num2 = NULL;
    const char *parse_end = NULL;
    char json[] = "{\"a\":1,\"b\":2}";
    size_t len = sizeof(json) - 1;
    double val_a = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    child = cJSON_GetObjectItem(parsed, "a");
    val_a = cJSON_GetNumberValue(child);
    num = cJSON_CreateNumber(val_a + 5.0);
    cJSON_AddItemToObject(root, "a_plus", num);
    dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "a_dup", dup);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "b");
    num2 = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "constant", num2);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
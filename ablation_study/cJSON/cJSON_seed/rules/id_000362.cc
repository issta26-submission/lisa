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
//<ID> 362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *json_text = "{\"alpha\":1, \"to_remove\":2}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    char *printed = NULL;
    cJSON *retrieved_parsed = NULL;
    double alpha_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(child, "count", 42.0);
    cJSON_AddItemToObject(root, "nested", child);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddNumberToObject(root, "top", 3.14);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    retrieved_parsed = cJSON_GetObjectItemCaseSensitive(root, "parsed");
    alpha_val = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(retrieved_parsed, "alpha"));
    buffer[0] = (char)('0' + (int)alpha_val);
    cJSON_DeleteItemFromObjectCaseSensitive(retrieved_parsed, "to_remove");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
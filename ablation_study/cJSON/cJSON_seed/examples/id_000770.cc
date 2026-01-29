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
//<ID> 770
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *initial_number = cJSON_CreateNumber(42.0);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddItemToObject(root, "count", initial_number);

    // step 2: Configure (serialize and prepare buffer)
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_count = cJSON_GetObjectItem(parsed, "count");
    double extracted = cJSON_GetNumberValue(parsed_count);
    cJSON *new_number = cJSON_CreateNumber(extracted + 0.5);
    cJSON_AddItemToObject(parsed, "count_plus_half", new_number);
    cJSON *fetched = cJSON_GetObjectItem(parsed, "count_plus_half");
    double validated = cJSON_GetNumberValue(fetched);
    (void)validated;
    (void)extracted;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
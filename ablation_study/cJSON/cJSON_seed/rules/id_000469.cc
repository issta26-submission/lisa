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
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *oldStatus = cJSON_CreateString("old_status");
    cJSON *newStatus = cJSON_CreateNumber(42.0);
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retrieved = NULL;
    double retrieved_number = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "status", oldStatus);
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddItemToObject(root, "meta", child);
    cJSON_AddStringToObject(child, "greet", "hello");
    cJSON_ReplaceItemInObject(root, "status", newStatus);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    retrieved = cJSON_GetObjectItem(parsed, "status");
    retrieved_number = cJSON_GetNumberValue(retrieved);
    buffer[0] = (char)('0' + ((int)retrieved_number % 10));
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
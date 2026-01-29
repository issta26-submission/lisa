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
//<ID> 441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"tester\",\"greeting\":\"hello\",\"values\":[1,2,3]}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *greet = cJSON_GetObjectItem(root, "greeting");
    const char *greet_text = cJSON_GetStringValue(greet);

    // step 2: Configure
    cJSON *greet_copy = cJSON_CreateString(greet_text);
    cJSON_AddItemToObject(root, "greeting_copy", greet_copy);
    cJSON_AddNullToObject(root, "optional");

    // step 3: Operate
    char *snapshot_before = cJSON_PrintBuffered(root, 256, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "optional");
    char *snapshot_after = cJSON_PrintBuffered(root, 128, 0);
    const char *copied_text = cJSON_GetStringValue(greet_copy);
    (void)copied_text;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_after);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
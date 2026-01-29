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
//<ID> 1341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"keep\": 42, \"remove_me\": false}";
    cJSON *root = NULL;
    cJSON *keep_item = NULL;
    cJSON *true_item = NULL;
    char *buffer1 = NULL;
    char *buffer2 = NULL;
    double keep_val = 0.0;
    int prebuffer = 128;
    cJSON_bool fmt = 1;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    keep_item = cJSON_GetObjectItem(root, "keep");
    keep_val = cJSON_GetNumberValue(keep_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "added_true", true_item);

    // step 3: Operate / Validate
    buffer1 = cJSON_PrintBuffered(root, prebuffer, fmt);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove_me");
    buffer2 = cJSON_PrintBuffered(root, prebuffer, fmt);
    (void)keep_val;
    (void)buffer1;
    (void)buffer2;

    // step 4: Cleanup
    cJSON_free(buffer1);
    cJSON_free(buffer2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
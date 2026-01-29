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
//<ID> 1345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"flag\": false, \"nested\": {\"keep\": true}}";
    cJSON *root = NULL;
    cJSON *nested = NULL;
    cJSON *flag_item = NULL;
    cJSON *true_item = NULL;
    char *printed1 = NULL;
    char *printed2 = NULL;
    int prebuffer = 128;
    cJSON_bool fmt = 1;
    cJSON_bool added_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    nested = cJSON_GetObjectItem(root, "nested");
    true_item = cJSON_CreateTrue();
    added_ok = cJSON_AddItemToObject(nested, "added_true", true_item);

    // step 3: Operate / Validate
    printed1 = cJSON_PrintBuffered(root, prebuffer, fmt);
    flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");
    printed2 = cJSON_PrintBuffered(root, prebuffer, fmt);

    // step 4: Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 847
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *true_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *fetched_raw_before = NULL;
    cJSON *fetched_raw_after = NULL;
    char *printed = NULL;
    char scratch[256];
    memset(scratch, 0, sizeof(scratch));
    const char *extracted_name = NULL;
    cJSON_bool raw_flag_before = 0;
    cJSON_bool raw_flag_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    true_item = cJSON_AddTrueToObject(root, "enabled");
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "raw_meta", raw_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    fetched_raw_before = cJSON_GetObjectItemCaseSensitive(root, "raw_meta");
    raw_flag_before = cJSON_IsRaw(fetched_raw_before);
    extracted_name = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(root, "name"));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "raw_meta");
    fetched_raw_after = cJSON_GetObjectItemCaseSensitive(root, "raw_meta");
    raw_flag_after = cJSON_IsRaw(fetched_raw_after);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(scratch, 0, sizeof(scratch));
    (void)extracted_name;
    (void)raw_flag_before;
    (void)raw_flag_after;
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1517
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
    cJSON *name_item = NULL;
    cJSON *ref_str = NULL;
    cJSON *fetched_ref = NULL;
    char *name_val = NULL;
    char *fetched_val = NULL;
    const char *parse_end = NULL;
    char json[] = "{ \"name\": \"Alice\", \"nested\": { \"x\": true }, \"values\": [1, 2, 3] }";

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItem(parsed, "name");
    name_val = cJSON_GetStringValue(name_item);
    ref_str = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObject(root, "name_ref", ref_str);
    fetched_ref = cJSON_GetObjectItem(root, "name_ref");
    fetched_val = cJSON_GetStringValue(fetched_ref);
    (void)fetched_val;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
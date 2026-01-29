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
//<ID> 1518
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *imported = NULL;
    cJSON *name_item = NULL;
    cJSON *name_ref = NULL;
    cJSON *ref_item = NULL;
    cJSON *ref = NULL;
    const char *parse_end = NULL;
    const char *name_val = NULL;
    const char *ref_val = NULL;
    char json[] = " { \"name\": \"Alice\", \"info\": { \"age\": 30 } } ";
    char prebuf[128];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    cJSON_AddStringToObject(root, "status", "ok");
    ref = cJSON_CreateStringReference("embedded_ref");
    cJSON_AddItemToObject(root, "ref", ref);

    // step 3: Operate / Validate
    imported = cJSON_GetObjectItem(root, "imported");
    name_item = cJSON_GetObjectItem(imported, "name");
    name_val = cJSON_GetStringValue(name_item);
    name_ref = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObject(root, "name_ref", name_ref);
    ref_item = cJSON_GetObjectItem(root, "ref");
    ref_val = cJSON_GetStringValue(ref_item);
    (void)parse_end;
    (void)prebuf;
    (void)ref_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
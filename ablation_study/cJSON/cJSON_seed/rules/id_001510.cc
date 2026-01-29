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
//<ID> 1510
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
    cJSON *name_item = NULL;
    cJSON *info = NULL;
    cJSON *greet_item = NULL;
    cJSON *strref = NULL;
    cJSON *dup_greet = NULL;
    const char *parse_end = NULL;
    const char *name_val = NULL;
    const char *greet_val = NULL;
    char json[] = "{ \"name\": \"Bob\", \"info\": { \"greet\": \"hello\" }, \"arr\": [\"x\",\"y\"] }";
    char prebuf[128];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    strref = cJSON_CreateStringReference("external_ref");
    cJSON_AddItemToObject(root, "external", strref);

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItem(parsed, "name");
    name_val = cJSON_GetStringValue(name_item);
    info = cJSON_GetObjectItem(parsed, "info");
    greet_item = cJSON_GetObjectItem(info, "greet");
    greet_val = cJSON_GetStringValue(greet_item);
    dup_greet = cJSON_CreateStringReference(greet_val);
    cJSON_AddItemToObject(root, "greet_ref", dup_greet);
    (void)name_val;
    (void)greet_val;
    (void)parse_end;
    (void)prebuf;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
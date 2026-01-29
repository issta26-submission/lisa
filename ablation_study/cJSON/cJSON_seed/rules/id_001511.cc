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
//<ID> 1511
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
    cJSON *imported = NULL;
    cJSON *nested = NULL;
    cJSON *label = NULL;
    cJSON *refstr = NULL;
    const char *parse_end = NULL;
    const char *label_val = NULL;
    const char *ref_val = NULL;
    char json[] = "{ \"name\": \"Bob\", \"nested\": { \"label\": \"inner\" } }";
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    refstr = cJSON_CreateStringReference("external-ref");
    cJSON_AddItemToObject(root, "ref", refstr);

    // step 3: Operate / Validate
    imported = cJSON_GetObjectItem(root, "imported");
    nested = cJSON_GetObjectItem(imported, "nested");
    label = cJSON_GetObjectItem(nested, "label");
    label_val = cJSON_GetStringValue(label);
    ref_val = cJSON_GetStringValue(refstr);
    (void)label_val;
    (void)ref_val;
    (void)parse_end;
    (void)scratch;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
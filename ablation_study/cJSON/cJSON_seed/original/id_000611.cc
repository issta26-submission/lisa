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
//<ID> 611
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(config, "threshold", 42.5);
    cJSON *numitem = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "primary", numitem);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(refs, numitem);
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    cJSON *threshold_item = cJSON_GetObjectItem(config, "threshold");
    double threshold = cJSON_GetNumberValue(threshold_item);
    cJSON *primary_item = cJSON_GetObjectItem(root, "primary");
    double primary_val = cJSON_GetNumberValue(primary_item);

    // step 3: Operate
    double combined_val = threshold + primary_val;
    cJSON *combined = cJSON_CreateNumber(combined_val);
    cJSON_AddItemToObject(root, "combined", combined);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *detached_config = cJSON_DetachItemFromObjectCaseSensitive(root, "config");
    cJSON_DeleteItemFromObject(root, "primary");

    // step 4: Validate & Cleanup
    cJSON *refs_after = cJSON_GetObjectItem(root, "refs");
    cJSON *ref0 = cJSON_GetArrayItem(refs_after, 0);
    double ref0_val = cJSON_GetNumberValue(ref0);
    cJSON_SetNumberHelper(combined, combined_val + ref0_val);
    double updated_val = cJSON_GetNumberValue(combined);
    (void)updated_val;
    cJSON_free(printed);
    cJSON_Delete(detached_config);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}